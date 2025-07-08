#include "../include/TwitchChatAPI.hpp"

#include "TwitchAuth.hpp"
#include "TwitchChat.hpp"


struct BadgeInfo::Impl {
    std::string name;
    int value;
};

BadgeInfo::BadgeInfo() : impl(std::make_unique<Impl>()) {}

BadgeInfo::BadgeInfo(const BadgeInfo& other) : impl(std::make_unique<Impl>(*other.impl)) {}

BadgeInfo::BadgeInfo(BadgeInfo&&) noexcept = default;

BadgeInfo& BadgeInfo::operator=(const BadgeInfo& other) {
    *impl = *other.impl;
    return *this;
}

BadgeInfo& BadgeInfo::operator=(BadgeInfo&&) noexcept = default;

BadgeInfo::~BadgeInfo() = default;

std::string BadgeInfo::getName() const { 
    return impl->name; 
}

int BadgeInfo::getValue() const { 
    return impl->value; 
}

void BadgeInfo::setName(std::string val) { 
    impl->name = std::move(val); 
}

void BadgeInfo::setValue(int val) { 
    impl->value = val; 
}

struct EmoteInfo::Impl {
    std::string ID;
    std::vector<Range> ranges;
};

EmoteInfo::EmoteInfo() : impl(std::make_unique<Impl>()) {}

EmoteInfo::EmoteInfo(const EmoteInfo& other) : impl(std::make_unique<Impl>(*other.impl)) {}

EmoteInfo::EmoteInfo(EmoteInfo&& other) noexcept = default;

EmoteInfo& EmoteInfo::operator=(const EmoteInfo& other) {
    if (this != &other)
        *impl = *other.impl;
    return *this;
}

EmoteInfo& EmoteInfo::operator=(EmoteInfo&& other) noexcept = default;

EmoteInfo::~EmoteInfo() = default;

std::string EmoteInfo::getID() const {
    return impl->ID;
}

const std::vector<Range>& EmoteInfo::getRanges() const {
    return impl->ranges;
}

void EmoteInfo::setID(std::string id) {
    impl->ID = std::move(id);
}

void EmoteInfo::setRanges(std::vector<Range> newRanges) {
    impl->ranges = std::move(newRanges);
}

struct ChatMessage::Impl {
    std::string username;
    std::string displayName;
    std::string message;
    std::string messageID;
    std::string userID;
    std::string clientNonce;
    std::string command;
    std::string rawMessage;
    std::vector<std::string> arguments;
    cocos2d::ccColor3B color;
    bool hasColor = false;
    bool isFirstMessage = false;
    bool isTurbo = false;
    bool isReturningChatter = false;
    bool isMod = false;
    bool isVIP = false;
    bool isSubscriber = false;
    int roomID = 0;
    unsigned long long int timestamp = 0;
    std::vector<BadgeInfo> badges;
    std::vector<EmoteInfo> emotes;
};

ChatMessage::ChatMessage()
    : impl(std::make_unique<Impl>()) {}

ChatMessage::ChatMessage(const ChatMessage& other)
    : impl(std::make_unique<Impl>(*other.impl)) {}

ChatMessage::ChatMessage(ChatMessage&& other) noexcept = default;

ChatMessage& ChatMessage::operator=(const ChatMessage& other) {
    if (this != &other) {
        *impl = *other.impl;
    }
    return *this;
}

ChatMessage& ChatMessage::operator=(ChatMessage&& other) noexcept = default;

ChatMessage::~ChatMessage() = default;

const std::string& ChatMessage::getUsername() const { 
    return impl->username; 
}

const std::string& ChatMessage::getDisplayName() const { 
    return impl->displayName; 
}

const std::string& ChatMessage::getMessage() const { 
    return impl->message; 
}

const std::string& ChatMessage::getMessageID() const { 
    return impl->messageID; 
}

const std::string& ChatMessage::getUserID() const { 
    return impl->userID; 
}

const std::string& ChatMessage::getClientNonce() const { 
    return impl->clientNonce; 
}

const std::string& ChatMessage::getCommand() const { 
    return impl->command; 
}

const std::string& ChatMessage::getRawMessage() const { 
    return impl->rawMessage; 
}

const std::vector<std::string>& ChatMessage::getArguments() const { 
    return impl->arguments; 
}

const cocos2d::ccColor3B& ChatMessage::getColor() const { 
    return impl->color; 
}

const std::vector<BadgeInfo>& ChatMessage::getBadges() const { 
    return impl->badges; 
}

const std::vector<EmoteInfo>& ChatMessage::getEmotes() const { 
    return impl->emotes; 
}

void ChatMessage::addBadge(BadgeInfo info) {
    impl->badges.push_back(info);
}

void ChatMessage::addEmote(EmoteInfo info) {
    impl->emotes.push_back(info);
}

bool ChatMessage::getHasColor() const { 
    return impl->hasColor; 
}

bool ChatMessage::getIsFirstMessage() const { 
    return impl->isFirstMessage; 
}

bool ChatMessage::getIsTurbo() const { 
    return impl->isTurbo; 
}

bool ChatMessage::getIsReturningChatter() const { 
    return impl->isReturningChatter; 
}

bool ChatMessage::getIsMod() const { 
    return impl->isMod; 
}

bool ChatMessage::getIsVIP() const { 
    return impl->isVIP; 
}

bool ChatMessage::getIsSubscriber() const { 
    return impl->isSubscriber; 
}

int ChatMessage::getRoomID() const { 
    return impl->roomID; 
}

unsigned long long int ChatMessage::getTimestamp() const { 
    return impl->timestamp; 
}

void ChatMessage::setUsername(std::string value) {
    impl->username = std::move(value);
}

void ChatMessage::setDisplayName(std::string value) {
    impl->displayName = std::move(value);
}

void ChatMessage::setMessage(std::string value) {
    impl->message = std::move(value);
}

void ChatMessage::setMessageID(std::string value) {
    impl->messageID = std::move(value);
}

void ChatMessage::setUserID(std::string value) {
    impl->userID = std::move(value);
}

void ChatMessage::setClientNonce(std::string value) {
    impl->clientNonce = std::move(value);
}

void ChatMessage::setCommand(std::string value) {
    impl->command = std::move(value);
}

void ChatMessage::setRawMessage(std::string value) {
    impl->rawMessage = std::move(value);
}

void ChatMessage::setArguments(std::vector<std::string> value) {
    impl->arguments = std::move(value);
}

void ChatMessage::setColor(cocos2d::ccColor3B value) {
    impl->color = value;
}

void ChatMessage::setHasColor(bool value) {
    impl->hasColor = value;
}

void ChatMessage::setIsFirstMessage(bool value) {
    impl->isFirstMessage = value;
}

void ChatMessage::setIsTurbo(bool value) {
    impl->isTurbo = value;
}

void ChatMessage::setIsReturningChatter(bool value) {
    impl->isReturningChatter = value;
}

void ChatMessage::setIsMod(bool value) {
    impl->isMod = value;
}

void ChatMessage::setIsVIP(bool value) {
    impl->isVIP = value;
}

void ChatMessage::setIsSubscriber(bool value) {
    impl->isSubscriber = value;
}

void ChatMessage::setRoomID(int value) {
    impl->roomID = value;
}

void ChatMessage::setTimestamp(unsigned long long int value) {
    impl->timestamp = value;
}

void ChatMessage::setBadges(std::vector<BadgeInfo> value) {
    impl->badges = std::move(value);
}

void ChatMessage::setEmotes(std::vector<EmoteInfo> value) {
    impl->emotes = std::move(value);
}

TwitchChatAPI* TwitchChatAPI::s_instance = nullptr;

struct TwitchChatAPI::Impl {
    std::vector<std::function<void(const ChatMessage&)>> chatMessageCallbacks;
    std::vector<std::function<void()>> onConnectedCallbacks;
    bool popupOpen = false;

    void runTokenCallback(std::function<void(const geode::Result<std::string>&)> callback, bool yes) {
        if (!callback) return;
        if (yes) callback(geode::Ok(TwitchChat::get()->getToken()));
        else callback(geode::Err("User declined token access."));
    }
};

void TwitchChatAPI::promptLogin(std::function<void(const geode::Result<std::string>&)> callback, bool force) {
    if ((!TwitchChat::loggedIn() || force) && !impl->popupOpen) {
        impl->popupOpen = true;
        geode::createQuickPopup("Twitch Login", "Authenticate <cg>Twitch Chat API</c> with Twitch?", "Cancel", "Ok", [this, callback] (auto, bool yes) {
            if (yes) {
                TwitchAuth::get()->loginTwitch([callback]() {
                    std::string token = Mod::get()->getSavedValue<std::string>("twitch-token");
                    std::string channel = Mod::get()->getSavedValue<std::string>("twitch-channel");
                    TwitchChat::resetInstance()->login(token, channel);
                    if (callback) callback(geode::Ok(channel));
                });
            }
            else {
                if (callback) callback(geode::Err("Login cancelled."));
            }
            
            impl->popupOpen = false;
        });
    }
}

TwitchChatAPI::TwitchChatAPI()
    : impl(std::make_unique<Impl>()) {
    s_instance = this;
}

TwitchChatAPI::~TwitchChatAPI() {
    if (s_instance == this)
        s_instance = nullptr;
}

std::vector<std::function<void(const ChatMessage&)>> TwitchChatAPI::getMessageCallbacks() {
    return impl->chatMessageCallbacks;
}

std::vector<std::function<void()>> TwitchChatAPI::getOnConnectedCallbacks() {
    return impl->onConnectedCallbacks;
}

void TwitchChatAPI::getToken(geode::Mod* mod, std::function<void(const geode::Result<std::string>&)> callback) {
    geode::createQuickPopup(
        "Twitch Chat API",
        fmt::format(
            "<cg>{}</c> wants to access your Twitch chat <cr>token</c>. "
            "This token has <cy>read only</c> permissions for <cb>chat messages</c>. "
            "Do you wish to grant access?",
            mod->getName()),
        "No", "Yes",
        [this, callback, mod](auto, bool yes) {
            Mod::get()->setSavedValue(mod->getID(), yes);
            runTokenCallback(callback, yes);
        },
        true
    );
}

void TwitchChatAPI::runTokenCallback(std::function<void(const geode::Result<std::string>&)> callback, bool yes) {
    impl->runTokenCallback(std::move(callback), yes);
}

void TwitchChatAPI::registerOnMessageCallback(std::function<void(const ChatMessage&)> callback) {
    impl->chatMessageCallbacks.push_back(std::move(callback));
}

void TwitchChatAPI::registerOnConnectedCallback(std::function<void()> callback) {
    impl->onConnectedCallbacks.push_back(std::move(callback));
}

bool TwitchChatAPI::isLoggedIn() {
    return TwitchChat::loggedIn();
}

bool TwitchChatAPI::modHasTokenPermission(geode::Mod* mod) {
    return Mod::get()->getSavedValue<bool>(mod->getID());
}

std::string TwitchChatAPI::getUsername(){
    return Mod::get()->getSavedValue<std::string>("twitch-channel");
}

TwitchChatAPI* TwitchChatAPI::get() {
    if (!s_instance) {
        s_instance = new TwitchChatAPI();
    }
    return s_instance;
}