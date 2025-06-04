#include <Geode/Geode.hpp>
#include <Geode/modify/GJDifficultySprite.hpp>

using namespace geode::prelude;

class $modify(ParticleGJDifficultySprite, GJDifficultySprite) {

    void makeParticle(GJFeatureState featureState) {
        
        ccColor3B rgb;
        
        switch (featureState) {
            case GJFeatureState::None:
            if (!Mod::get()->getSettingValue<bool>("show-rate")) return;
            rgb = Mod::get()->getSettingValue<cocos2d::ccColor3B>("rate-color");
            break;
            
            case GJFeatureState::Featured:
            if (!Mod::get()->getSettingValue<bool>("show-featured")) return;
            rgb = Mod::get()->getSettingValue<cocos2d::ccColor3B>("featured-color");
            break;
            
            case GJFeatureState::Epic:
            if (!Mod::get()->getSettingValue<bool>("show-epic")) return;
            rgb = Mod::get()->getSettingValue<cocos2d::ccColor3B>("epic-color");
            break;
            
            case GJFeatureState::Legendary:
            if (!Mod::get()->getSettingValue<bool>("show-legendary")) return;
            rgb = Mod::get()->getSettingValue<cocos2d::ccColor3B>("legendary-color");
            break;
            
            case GJFeatureState::Mythic:
            if (!Mod::get()->getSettingValue<bool>("show-mythic")) return;
            rgb = Mod::get()->getSettingValue<cocos2d::ccColor3B>("mythic-color");
            break;
        }
        
        if (getChildByID("rating-particles"_spr)) getChildByID("rating-particles"_spr)->removeFromParentAndCleanup(true);

        ccColor3B endrgb = ccc3(std::clamp(rgb.r + 30, 0, 255), std::clamp(rgb.g + 30, 0, 255), std::clamp(rgb.b + 30, 0, 255));
        std::string startColor = fmt::format("{}a0a{}a0a{}a0a", rgb.r / 255.0f, rgb.g / 255.0f, rgb.b / 255.0f);
        std::string endColor = fmt::format("{}a0a{}a0a{}a0a", endrgb.r / 255.0f, endrgb.g / 255.0f, endrgb.b / 255.0f);
        
        int amount = Mod::get()->getSettingValue<int64_t>("amount");
        int speed = Mod::get()->getSettingValue<int64_t>("speed");
        int size = Mod::get()->getSettingValue<int64_t>("size");
        int texture = Mod::get()->getSettingValue<int64_t>("texture");
        int blending = (int)Mod::get()->getSettingValue<bool>("blending");
        
        std::string pString = fmt::format("{}a-1a1.3a0.2a{}a90a0a{}a5a20a20a0a0a8a0a0a0a{}a1a0a0a{}1a0a1a1a0a 0a{}1a0a0.27a0a0.27a0a0a0a0a0a0a0a0a2a{}a0a0a0a{}a0a0a0.25a0a 0a0a0a0a0a0a0a0a0a0", amount, amount, speed, size, startColor, endColor, blending, texture);
        
        ParticleStruct pStruct;
        GameToolbox::particleStringToStruct(pString, pStruct);
        CCParticleSystemQuad* particle = GameToolbox::particleFromStruct(pStruct, nullptr, false);
        addChild(particle, -3);
        particle->resetSystem();
        particle->setPosition(getContentSize() / 2.f);
        particle->setID("rating-particles"_spr);

        particle->update(0.15f);
        particle->update(0.15f);
        particle->update(0.15f);
        particle->update(0.15f);
        particle->update(0.15f);
    }

    static GJDifficultySprite* create(int p0, GJDifficultyName name) {
        GJDifficultySprite* sprite = GJDifficultySprite::create(p0, name);
        static_cast<ParticleGJDifficultySprite*>(sprite)->makeParticle(sprite->m_featureState);
        return sprite;
    }

    void updateFeatureState(GJFeatureState featureState) {
        GJDifficultySprite::updateFeatureState(featureState);

        if (featureState == GJFeatureState::Mythic) {
            CCParticleSystemQuad* ogParticle = getChildByType<CCParticleSystemQuad*>(0);
            if (ogParticle) ogParticle->setVisible(false);
        }

        makeParticle(featureState);
    }
};
