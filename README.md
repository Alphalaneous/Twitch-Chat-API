# Twitch Chat API

An API for interacting with Twitch Chat. 

Users must authenticate with their Twitch Account for this mod to be able to see chat messages in their chat. This can be handled by third party mods or within this mod's settings. 

<cr>YouTube support is **not planned**, please do not ask about it.</c>

# Getting Started

Prompting the user to log in is easy, just call `TwitchChatAPI::get()->promptLogin()`, and if you wish for it to always prompt, even when there is an account logged in, pass in `true` to force it. You can call this whenever, and it is recommended to have some way to prompt it so users do not have to go to this mod's settings to manage their account.

## Message Listening

You can register a callback for messages by using `TwitchChatAPI::get()->registerOnMessageCallback(std::function<void(const ChatMessage&)>)` which takes in a lambda with a ChatMessage parameter. An example would be:

```cpp
TwitchChatAPI::get()->registerOnMessageCallback([] (const ChatMessage& chatMessage) {
    if (chatMessage.getCommand() == "!meow") {
        log::info("mrrp");
    }
});
```

## Connection Listening

Whenever an account is logged in via this API, the on connected callback is ran, so you can gracefully handle changes to account, or initialize something when connected. This is using `TwitchChatAPI::get()->registerOnConnectedCallback(std::function<void()> callback)`. An example would be:

```cpp
TwitchChatAPI::get()->registerOnConnectedCallback([] () {
    log::info("Connected!");
});
```

## Obtaining A Read Only Chat Token

You do **not** need to use this unless you **need** direct access to the token. This method is **not** a log in method, and if you have to use it, must be used after a log in.

Sometimes you may need to send a token somewhere else for verification, this mod allows this, but will prompt the user if they wish to agree to it. This token cannot do anything more than grab basic user information and read chat messages, and cannot access or modify sensitive information. If you need more access than this gives, you will need to set up your own log in flow to generate your own token.

This method returns a string result, if the user agreed, the token will be given, else it will return an error result and cannot be gotten.

```cpp
TwitchChatAPI::get()->getToken([] (const geode::Result<std::string>& token) {
    log::info("token {}", token.unwrapOr("err"));
});
```

## ChatMessage Methods

```cpp
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
```

## EmoteInfo Methods

```cpp
std::string getID() const;
const std::vector<Range>& getRanges() const;
void setID(std::string);
void setRanges(std::vector<Range>);
```

## BadgeInfo Methods

```cpp
std::string getName() const;
int getValue() const;
void setName(std::string);
void setValue(int);
```

## Range Struct

Simple struct that contains a range, used here for emote positions in the message.

```cpp
int start;
int end;
```
