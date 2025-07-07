#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "TwitchChat.hpp"

using namespace geode::prelude;

$execute {
	if (Mod::get()->hasSavedValue("twitch-token") && Mod::get()->hasSavedValue("twitch-channel")) {
		std::string token = Mod::get()->getSavedValue<std::string>("twitch-token");
		std::string channel = Mod::get()->getSavedValue<std::string>("twitch-channel");
		TwitchChat::resetInstance()->login(token, channel);
	}
}
