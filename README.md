# Twitch Chat API

An API for interacting with Twitch Chat. 

Users must authenticate with their Twitch Account for this mod to be able to see chat messages in their chat. This can be handled by third party mods or within this mod's settings. 

<cr>YouTube support is **not planned**, please do not ask about it.</c>

# Getting Started

Prompting the user to log in is easy, just call `TwitchChatAPI::get()->promptLogin(std::function<void(const geode::Result<std::string>&)> callback = nullptr, bool force = false)`, and if you wish for it to always prompt, even when there is an account logged in, pass in `true` to force it. You can call this whenever, and it is recommended to have some way to prompt it so users do not have to go to this mod's settings to manage their account. Examples:

```cpp
TwitchChatAPI::get()->promptLogin();
```

```cpp
TwitchChatAPI::get()->promptLogin([] (const Result<std::string>& channel) {
    log::info("{}", channel.unwrapOr("unknown"));
});
```

```cpp
TwitchChatAPI::get()->promptLogin(nullptr, true);
```

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

Sometimes you may need to send a token somewhere else for verification, this mod allows this, but will prompt the user if they wish to agree to it. This token cannot do anything more than grab basic user information and read chat messages, and cannot access or modify sensitive information. If you need more access than this gives, you will need to set up your own log in flow to generate your own token. You must pass in your mod to the method, per the example.

This method returns a string result, if the user agreed, the token will be given, else it will return an error result and cannot be gotten.

```cpp
TwitchChatAPI::get()->getToken(Mod::get(), [] (const Result<std::string>& token) {
    log::info("Token: {}", token.unwrapOr("err"));
});
```

Alongside that, you may need to know whenever a token is changed, which you can register a callback for. This callback will only be recieved by mods that have permission to access the token. Example:

```cpp
TwitchChatAPI::get()->registerTokenChangeCallback(Mod::get(), [] (const std::string& token) {
    log::info("Token: {}", token);
});
```

## TwitchChatAPI Methods

```cpp
bool isLoggedIn(); // returns true if user is logged in
bool modHasTokenPermission(Mod* mod); // returns true if user has granted mod token access
std::string getUsername(); // returns the logged in channel's username
```

## ChatMessage Methods

```cpp

// Getters

const std::string& getUsername() const; // returns the chatter's username
const std::string& getDisplayName() const; // returns the chatter's display name
const std::string& getMessage() const; // returns the chat message
const std::string& getMessageID() const; // returns the chat message ID
const std::string& getUserID() const;  // returns the chatter's user ID
const std::string& getClientNonce() const; // returns the client nonce
const std::string& getCommand() const; // returns the command in a chat message (first word when separated by spaces)
const std::string& getRawMessage() const;  // returns the raw chat message from IRC
const std::vector<std::string>& getArguments() const;  // returns the arguments in a chat message (all but first word when separated by spaces)
const cocos2d::ccColor3B& getColor() const; // returns the chatter's chat color
bool getHasColor() const;  // returns true if the chatter has a chat color
bool getIsFirstMessage() const; // returns true if this this the chatter's first message in the channel
bool getIsTurbo() const; // returns true if the chatter has Twitch Turbo
bool getIsReturningChatter() const; // returns true if the chatter is a returning chatter
bool getIsMod() const; // returns true if the chatter is a moderator
bool getIsVIP() const; // returns true if the chatter is a VIP
bool getIsSubscriber() const; // returns true if the chatter is a subscriber
int getRoomID() const;  // returns the room ID of the channel
unsigned long long int getTimestamp() const; // returns the timestamp of the message
const std::vector<BadgeInfo>& getBadges() const; // returns a vector of Badge information of the chatter
const std::vector<EmoteInfo>& getEmotes() const; // returns a vector of Emote information of the message

// Setters

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
