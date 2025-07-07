#pragma once
#include "libraries/httplib.h"
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class TwitchAuth {

protected:
    static TwitchAuth* instance;
    httplib::Server m_server;
    std::function<void()> m_callback;
    int m_id;
    std::map<int, EventListener<web::WebTask>> m_listeners;
    std::unordered_map<std::filesystem::path, std::pair<ByteVector, std::string>> m_files;

public:
    void start();
    void stop();
    void loginTwitch(std::function<void()>);
    void setChannel(std::string token);
    static TwitchAuth* get(){
        if (!instance) {
            instance = new TwitchAuth();
            instance->start();
        };
        return instance;
    }
};