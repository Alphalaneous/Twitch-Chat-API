#include "TwitchChat.hpp"

#define method(functionName) std::bind(&TwitchChat::functionName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

TwitchChat* TwitchChat::s_currentInstance = nullptr;

TwitchChat* TwitchChat::resetInstance() {
    if (s_currentInstance) {
        s_currentInstance->close();
        delete s_currentInstance;
    }
    s_currentInstance = new TwitchChat();
    return s_currentInstance;
}

TwitchChat* TwitchChat::get() {
    return s_currentInstance;
}

std::string TwitchChat::getToken() {
    return m_token;
}

void TwitchChat::login(const std::string& token, const std::string& channelName) {
    m_token = token;
    m_channelName = channelName;
    connectAsync();
}

bool TwitchChat::loggedIn() {
    return s_currentInstance;
}

void TwitchChat::connect() {
    m_webSocket = WebSocket::from_url("ws://irc-ws.chat.twitch.tv:80");
    if (!m_webSocket) {
        delete m_webSocket;
        return;
    }

    for (const auto& callback : TwitchChatAPI::get()->getOnConnectedCallbacks()) {
        geode::queueInMainThread([=] {
            callback();
        });
    }

    send("CAP REQ :twitch.tv/tags");
    send("CAP REQ :twitch.tv/commands");
    send("CAP REQ :twitch.tv/membership");
    send("PASS oauth:" + m_token);
    send("NICK GeodeBot");
    send("JOIN #" + m_channelName);

    while (m_webSocket->getReadyState() != WebSocket::CLOSED) {
        m_webSocket->poll();
        m_webSocket->dispatch([&](const std::string& message) {
            onMessage(message);
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    delete m_webSocket;
}

void TwitchChat::connectAsync() {
    std::thread t = std::thread([&] {
        connect();
    });
    t.detach();
}

std::string TwitchChat::getChannelName() {
    return m_channelName;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter, int limit = -1) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);
    int count = 0;

    while (end != std::string::npos && (count < limit - 1 || limit == -1)) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
        count++;
    }

    result.push_back(str.substr(start));
    return result;
}

geode::Result<ChatMessage> TwitchChat::parseMessage(const std::string& message) {
    std::vector<std::string> splitAt = split(message, "@", 3);
    std::vector<std::string> splitMsg = split(message, "PRIVMSG", 2);

    if (splitAt.size() < 3) return geode::Err("Invalid Message");
    if (splitMsg.size() < 2) return geode::Err("Invalid Message");

    std::string tagsPrefix = splitAt[1];
    std::string channelPrefix = splitAt[2];
    std::string sentMessage = geode::utils::string::trim(split(splitMsg[1], ":", 2)[1]);
    std::string sender = split(channelPrefix, ".tmi.twitch.tv")[0];
    ChatMessage chatMessage;
    chatMessage.setUsername(sender);
    chatMessage.setDisplayName(sender);
    chatMessage.setMessage(sentMessage);
    chatMessage.setRawMessage(message);

    std::vector<std::string> commandParts = split(sentMessage, " ", 2);
    if (commandParts.size() > 0) {
        chatMessage.setCommand(commandParts[0]);
        if (commandParts.size() > 1) {
            chatMessage.setArguments(split(commandParts[1], " "));
        }
    }

    std::vector<std::string> tags = split(tagsPrefix, ";");

    for (const auto& tag : tags) {
        std::vector<std::string> tagParts = split(tag, "=", 2);

        if (tagParts[0] == "display-name") {
            chatMessage.setDisplayName(tagParts[1]);
        }
        if (tagParts[0] == "color") {
            chatMessage.setColor(geode::cocos::cc3bFromHexString(tagParts[1]).unwrapOrDefault());
            chatMessage.setHasColor(true);
        }
        if (tagParts[0] == "client-nonce") {
            chatMessage.setClientNonce(tagParts[1]);
        }
        if (tagParts[0] == "badges") {
            std::vector<std::string> badges = split(tagParts[1], ",");
            for (const auto& badge : badges) {
                std::vector<std::string> badgeParts = split(badge, "/", 2);
                if (badgeParts.size() < 2) continue;
                BadgeInfo badgeInfo;
                badgeInfo.setName(badgeParts[0]);
                badgeInfo.setValue(geode::utils::numFromString<int>(badgeParts[1]).unwrapOrDefault());

                chatMessage.addBadge(badgeInfo);
            }
        }
        if (tagParts[0] == "emotes") {
            std::vector<std::string> emotes = split(tagParts[1], "/");
            for (const auto& emote : emotes) {
                std::vector<std::string> emoteParts = split(emote, ":", 2);
                if (emoteParts.size() < 2) continue;
                std::vector<std::string> ranges = split(emoteParts[1], ",");
                std::vector<Range> rangeData;
                for (const auto& range : ranges) {
                    std::vector<std::string> rangeParts = split(range, "-", 2);
                    rangeData.push_back(
                        {
                            geode::utils::numFromString<int>(rangeParts[0]).unwrapOrDefault(), 
                            geode::utils::numFromString<int>(rangeParts[1]).unwrapOrDefault()
                        }
                    );
                }

                EmoteInfo emoteInfo;
                emoteInfo.setID(emoteParts[0]);
                emoteInfo.setRanges(rangeData);

                chatMessage.addEmote(emoteInfo);
            }
        }
        if (tagParts[0] == "id") {
            chatMessage.setMessageID(tagParts[1]);
        }
        if (tagParts[0] == "user-id") {
            chatMessage.setUserID(tagParts[1]);
        }
        if (tagParts[0] == "returning-chatter") {
            chatMessage.setIsReturningChatter(geode::utils::numFromString<int>(tagParts[1]).unwrapOrDefault());
        }
        if (tagParts[0] == "tmi-sent-ts") {
            chatMessage.setTimestamp(geode::utils::numFromString<unsigned long long int>(tagParts[1]).unwrapOrDefault());
        }
        if (tagParts[0] == "turbo") {
            chatMessage.setIsTurbo(geode::utils::numFromString<int>(tagParts[1]).unwrapOrDefault());
        }
        if (tagParts[0] == "first-msg") {
            chatMessage.setIsFirstMessage(geode::utils::numFromString<int>(tagParts[1]).unwrapOrDefault());
        }
        if (tagParts[0] == "vip") {
            chatMessage.setIsVIP(geode::utils::numFromString<int>(tagParts[1]).unwrapOrDefault());
        }
    }

    for (const auto& badge : chatMessage.getBadges()) {
        if (badge.getName() == "broadcaster" || badge.getName() == "moderator") chatMessage.setIsMod(true);
        if (badge.getName() == "subscriber" || badge.getName() == "founder") chatMessage.setIsSubscriber(true);
    }
   
    return geode::Ok(chatMessage);
}

void TwitchChat::onMessage(std::string message) {
    geode::utils::string::replaceIP(message, "\r", "");
    geode::utils::string::replaceIP(message, "\n", "");

    if (geode::utils::string::contains(message, "PRIVMSG")) {
        geode::Result<ChatMessage> chatMessageOpt = parseMessage(message);
        if (chatMessageOpt.isOk()) {
            ChatMessage chatMessage = chatMessageOpt.unwrap();
            for (const auto& callback : TwitchChatAPI::get()->getMessageCallbacks()) {
                geode::queueInMainThread([=] {
                    callback(chatMessage);
                });
            }
        }
    }

    if (message == "PING :tmi.twitch.tv") {
        send("PONG :tmi.twitch.tv");
    }
}

void TwitchChat::send(const std::string& message) {
    m_webSocket->send(message);
}

void TwitchChat::close() {
    m_webSocket->close();
}