#pragma once
#include <Geode/Geode.hpp>


#ifdef GEODE_IS_WINDOWS
    #ifdef ALPHALANEOUS_TWITCH_CHAT_API_EXPORTING
        #define ALPHA_TWITCH_CHAT_API_DLL __declspec(dllexport)
    #else
        #define ALPHA_TWITCH_CHAT_API_DLL __declspec(dllimport)
    #endif
#else
    #define ALPHA_TWITCH_CHAT_API_DLL __attribute__((visibility("default")))
#endif

struct ALPHA_TWITCH_CHAT_API_DLL Range {
    int start;
    int end;
};

struct ALPHA_TWITCH_CHAT_API_DLL BadgeInfo {
    struct Impl;
    std::unique_ptr<Impl> impl;

    BadgeInfo();
    BadgeInfo(const BadgeInfo&);
    BadgeInfo(BadgeInfo&&) noexcept;
    BadgeInfo& operator=(const BadgeInfo&);
    BadgeInfo& operator=(BadgeInfo&&) noexcept;
    ~BadgeInfo();

    std::string getName() const;
    int getValue() const;
    void setName(std::string);
    void setValue(int);
};

struct ALPHA_TWITCH_CHAT_API_DLL EmoteInfo {
    struct Impl;
    std::unique_ptr<Impl> impl;

    EmoteInfo();
    EmoteInfo(const EmoteInfo&);
    EmoteInfo(EmoteInfo&&) noexcept;
    EmoteInfo& operator=(const EmoteInfo&);
    EmoteInfo& operator=(EmoteInfo&&) noexcept;
    ~EmoteInfo();

    std::string getID() const;
    const std::vector<Range>& getRanges() const;
    void setID(std::string);
    void setRanges(std::vector<Range>);
};

struct ALPHA_TWITCH_CHAT_API_DLL ChatMessage {
    struct Impl;
    std::unique_ptr<Impl> impl;

    ChatMessage();
    ChatMessage(const ChatMessage&);
    ChatMessage(ChatMessage&&) noexcept;
    ChatMessage& operator=(const ChatMessage&);
    ChatMessage& operator=(ChatMessage&&) noexcept;
    ~ChatMessage();

    const std::string& getUsername() const;
    const std::string& getDisplayName() const;
    const std::string& getMessage() const;
    const std::string& getMessageID() const;
    const std::string& getUserID() const;
    const std::string& getClientNonce() const;
    const std::string& getCommand() const;
    const std::string& getRawMessage() const;
    const std::vector<std::string>& getArguments() const;
    const cocos2d::ccColor3B& getColor() const;
    bool getHasColor() const;
    bool getIsFirstMessage() const;
    bool getIsTurbo() const;
    bool getIsReturningChatter() const;
    bool getIsMod() const;
    bool getIsVIP() const;
    bool getIsSubscriber() const;
    int getRoomID() const;
    unsigned long long int getTimestamp() const;
    const std::vector<BadgeInfo>& getBadges() const;
    const std::vector<EmoteInfo>& getEmotes() const;

    void setUsername(std::string);
    void setDisplayName(std::string);
    void setMessage(std::string);
    void setMessageID(std::string);
    void setUserID(std::string);
    void setClientNonce(std::string);
    void setCommand(std::string);
    void setRawMessage(std::string);
    void setArguments(std::vector<std::string>);
    void setColor(cocos2d::ccColor3B);
    void setHasColor(bool);
    void setIsFirstMessage(bool);
    void setIsTurbo(bool);
    void setIsReturningChatter(bool);
    void setIsMod(bool);
    void setIsVIP(bool);
    void setIsSubscriber(bool);
    void setRoomID(int);
    void setTimestamp(unsigned long long int);
    void setBadges(std::vector<BadgeInfo>);
    void setEmotes(std::vector<EmoteInfo>);
    void addBadge(BadgeInfo info);
    void addEmote(EmoteInfo info);
};

class ALPHA_TWITCH_CHAT_API_DLL TwitchChatAPI {
public:
    TwitchChatAPI();
    ~TwitchChatAPI();
    TwitchChatAPI(const TwitchChatAPI&) = delete;
    TwitchChatAPI& operator=(const TwitchChatAPI&) = delete;

    static TwitchChatAPI* get();

    void promptLogin(bool force = false);

    inline void getToken(std::function<void(const geode::Result<std::string>&)> callback) {
        geode::Mod* mod = geode::Mod::get();
        geode::createQuickPopup(
            "Twitch Chat API",
            fmt::format(
                "<cg>{}</c> wants to access your Twitch chat <cr>token</c>. "
                "This token has <cy>read only</c> permissions for <cb>chat messages</c>. "
                "Do you wish to grant access?",
                mod->getName()),
            "No", "Yes",
            [this, callback](auto, bool yes) {
                runTokenCallback(callback, yes);
            },
            true
        );
    }

    void registerOnMessageCallback(std::function<void(const ChatMessage&)> callback);
    void registerOnConnectedCallback(std::function<void()> callback);
    std::vector<std::function<void(const ChatMessage&)>> getMessageCallbacks();
    std::vector<std::function<void()>> getOnConnectedCallbacks();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;

    static TwitchChatAPI* s_instance;

    void runTokenCallback(std::function<void(const geode::Result<std::string>&)> callback, bool yes);

    friend class TwitchChat;
};
