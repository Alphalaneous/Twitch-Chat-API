#include "TwitchAuth.hpp"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

TwitchAuth* TwitchAuth::instance = nullptr;

//for the sake of if I wanna copy this for another mod that has a web server in the future haha
std::string getMimeType(const std::filesystem::path& path) {
    static const std::unordered_map<std::string, std::string> mimeTypes = {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".png", "image/png"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".gif", "image/gif"},
        {".svg", "image/svg+xml"},
        {".txt", "text/plain"},
    };

    auto ext = path.extension().string();
    geode::utils::string::toLowerIP(ext);

    auto it = mimeTypes.find(ext);
    if (it != mimeTypes.end())
        return it->second;

    return "application/octet-stream";
}

void TwitchAuth::start() {

    std::filesystem::path resources = Mod::get()->getResourcesDir();

    if (std::filesystem::exists(resources) && std::filesystem::is_directory(resources)) {
        for (const auto& entry : std::filesystem::directory_iterator(resources)) {
            if (std::filesystem::is_regular_file(entry.status())) {

                std::string contentType = getMimeType(entry);
                Result<ByteVector> res = geode::utils::file::readBinary(entry);
                if (res.isOk()) {
                    m_files[entry] = {res.unwrap(), contentType};
                    
                }
            }
        }
    }

    std::thread thread([this] {
        for (const auto& [k, v] : m_files) {
            m_server.Get(fmt::format("/{}", k.filename()), [this, k, v](const httplib::Request &req, httplib::Response &res) {
                if (k.filename() == "auth.html" && req.has_param("access_token")) {
                    std::string token = req.get_param_value("access_token");
                    queueInMainThread([this, token] {
                        Mod::get()->setSavedValue("twitch-token", token);
                        setChannel(token);
                    });
                    return;
                }
                res.set_content(reinterpret_cast<const char*>(v.first.data()), v.first.size(), v.second);
            });
        }
        m_server.listen("localhost", 23523);
    });
    thread.detach();
}

void TwitchAuth::setChannel(std::string token) {

    auto req = web::WebRequest();
    req.header("Authorization", "Bearer " + Mod::get()->getSavedValue<std::string>("twitch-token"));
    req.header("Client-ID", "6pk13igddtqdcbqhr37ndptg2qd5uv");

    int id = m_id;
    m_id++;
    m_listeners[id].bind([&](web::WebTask::Event* e){
        if (auto res = e->getValue()) {

            if (res->ok() && res->json().isOk()) {
                matjson::Value channelInfo = res->json().unwrap();
                std::vector<matjson::Value> values;
                Result<std::vector<matjson::Value>&> dataRes = channelInfo["data"].asArray();
                if (!dataRes.isOk()) return;
                values = dataRes.unwrap();
                matjson::Value firstObject = values.at(0);
                std::string login = firstObject["login"].asString().unwrapOrDefault();
                if (login.empty()) return;
                login = utils::string::replace(login, "\"", "");
                Mod::get()->setSavedValue("twitch-channel", login);
                m_callback();
            }
        }
        m_listeners.erase(id);
    });

    auto webTask = req.get("https://api.twitch.tv/helix/users");

    m_listeners[id].setFilter(webTask);
}

void TwitchAuth::stop() {
    m_server.stop();
}

void TwitchAuth::loginTwitch(std::function<void()> callback) {
    m_callback = callback;
    std::string authlink = "https://id.twitch.tv/oauth2/authorize?response_type=token&client_id=6pk13igddtqdcbqhr37ndptg2qd5uv&redirect_uri=http://localhost:23523/auth.html&force_verify=true&scope=";

    std::vector<std::string> params = {
        "chat:read"
    };

    for (int i = 0; i < params.size(); i++) {
        std::string param = params[i];
        authlink += param;
        if (i < params.size() - 1) authlink += "+";
    }

    web::openLinkInBrowser(authlink);
}
