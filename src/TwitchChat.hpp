#pragma once
#include <Geode/Geode.hpp>
#include "libraries/easywsclient.hpp"
#include "../include/TwitchChatAPI.hpp"

using namespace easywsclient;

class TwitchChat {

private:
    WebSocket::pointer m_webSocket;
    std::string m_token;
    std::string m_channelName;
    std::unordered_map<std::string, std::function<std::string(const ChatMessage&, const std::string&, const std::vector<std::string>&)>> m_commands;
    geode::Result<ChatMessage> parseMessage(const std::string& message);
    void connect();
    void connectAsync();
    void onMessage(std::string message);
    void send(const std::string& message);
    void close();
public:
    static TwitchChat* s_currentInstance;
    static TwitchChat* resetInstance();
    static TwitchChat* get();
    static bool loggedIn();
    std::string getToken();
    std::string getChannelName();
    void login(const std::string& token, const std::string& channelName);
};