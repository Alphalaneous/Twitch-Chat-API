#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include "TwitchAuth.hpp"
#include "TwitchChat.hpp"

using namespace geode::prelude;

class ButtonSettingV3 : public SettingV3 {
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
        auto res = std::make_shared<ButtonSettingV3>();
        auto root = checkJson(json, "ButtonSettingV3");

        res->init(key, modID, root);
        res->parseNameAndDescription(root);
        res->parseEnableIf(root);
        
        root.checkUnknownKeys();
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }

    bool isDefaultValue() const override {
        return true;
    }
    void reset() override {}

    SettingNodeV3* createNode(float width) override;
};

class ButtonSettingNodeV3 : public SettingNodeV3 {
protected:
    ButtonSprite* m_buttonSprite;
    CCMenuItemSpriteExtra* m_button;
    CCLabelBMFont* m_accountNameLabel;

    bool init(std::shared_ptr<ButtonSettingV3> setting, float width) {
        if (!SettingNodeV3::init(setting, width))
            return false;
    
        std::string signinText = "Sign In";
        if (TwitchChat::loggedIn()) {
            signinText = "Re-sign In";
        }

        m_buttonSprite = ButtonSprite::create(signinText.c_str(), "goldFont.fnt", "GJ_button_01.png", .8f);
        m_buttonSprite->setScale(.5f);
        m_button = CCMenuItemSpriteExtra::create(
            m_buttonSprite, this, menu_selector(ButtonSettingNodeV3::onButton)
        );
        this->getButtonMenu()->addChildAtPosition(m_button, Anchor::Center);
        this->getButtonMenu()->setContentWidth(m_button->getScaledContentWidth());
        this->getButtonMenu()->updateLayout();

        if (TwitchChat::loggedIn()) {
            m_accountNameLabel = CCLabelBMFont::create(fmt::format("Account: {}", Mod::get()->getSavedValue<std::string>("twitch-channel")).c_str(), "bigFont.fnt");
        }
        else {
            m_accountNameLabel = CCLabelBMFont::create("Not Signed In", "bigFont.fnt");
        }

        m_accountNameLabel->setAnchorPoint({0, 0.5f});
        m_accountNameLabel->setPosition({8, getContentSize().height/2});
        m_accountNameLabel->setScale(0.5f);

        this->getNameMenu()->removeAllChildren();
        this->addChild(m_accountNameLabel);

        this->updateState(nullptr);
        
        return true;
    }
    
    void updateState(CCNode* invoker) override {
        // no-op
    }

    void onButton(CCObject*) {
        TwitchAuth::get()->loginTwitch([this]() {
			std::string token = Mod::get()->getSavedValue<std::string>("twitch-token");
			std::string channel = Mod::get()->getSavedValue<std::string>("twitch-channel");
			TwitchChat::resetInstance()->login(token, channel);
            m_accountNameLabel->setString(fmt::format("Account: {}", channel).c_str());
		});
    }

    void onCommit() override {}
    void onResetToDefault() override {}

public:
    static ButtonSettingNodeV3* create(std::shared_ptr<ButtonSettingV3> setting, float width) {
        auto ret = new ButtonSettingNodeV3();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool hasUncommittedChanges() const override {
        return false;
    }
    bool hasNonDefaultValue() const override {
        return false;
    }

    std::shared_ptr<ButtonSettingV3> getSetting() const {
        return std::static_pointer_cast<ButtonSettingV3>(SettingNodeV3::getSetting());
    }
};

SettingNodeV3* ButtonSettingV3::createNode(float width) {
    return ButtonSettingNodeV3::create(
        std::static_pointer_cast<ButtonSettingV3>(shared_from_this()),
        width
    );
}

$execute {
    (void)Mod::get()->registerCustomSettingType("button", &ButtonSettingV3::parse);
}