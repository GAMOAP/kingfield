//
//  GameInfoLayer.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/09/2019.
//

#include "GameInfoLayer.hpp"

#include "GameDirector.hpp"

#include "MainGrid.hpp"
#include "MainObject.hpp"

#include "MainSounds.hpp"

#include <iostream>

USING_NS_CC;

GameInfoLayer* GameInfoLayer::getInstance()
{
    if (!m_SharedGameInfoLayer)
    {
        m_SharedGameInfoLayer = new (std::nothrow) GameInfoLayer;
        m_SharedGameInfoLayer->init();
    }
    return m_SharedGameInfoLayer;
}

bool GameInfoLayer::init()
{
    m_infoLayer = MainObject::getInfoLayer();
    
    return true;
}

//------------------------INTRO TITLE---------------------------------
void GameInfoLayer::addIntroTitle()
{
    m_SharedGameInfoLayer->initIntroTitle(true);
    MainSounds::playGame("tittle");
}
void GameInfoLayer::removeIntroTitle()
{
    m_SharedGameInfoLayer->initIntroTitle(false);
}
void GameInfoLayer::initIntroTitle(bool addRemove)
{
    if(!addRemove)
    {
        if(m_introLogo)
        {
            auto scale = ScaleTo::create(1.2, 5);
            auto fadeOut = FadeOut::create(1);
            auto spawn = Spawn::createWithTwoActions(scale, fadeOut);
            auto easeIn = EaseIn::create(spawn, 0.4);
            auto callFunc = CallFunc::create([=]()
            {
                m_introLogo->removeFromParent();
                m_introLogo = nullptr;
            });
            auto seq = Sequence::create(easeIn, callFunc, NULL);
            m_introLogo->runAction(seq);
        }
    }
    else
    {
        m_introLogo = Sprite::createWithSpriteFrameName("screen/logo.png");
        Vec2 visibleSize = MainGrid::getVisibleSize();
        m_introLogo->setPosition(Vec2(visibleSize.x/2, visibleSize.y/2));
        m_infoLayer->addChild(m_introLogo);
    }
}

//------------------------CONNECT SPRITE------------------------------
void GameInfoLayer::addConnectSprite()
{
    m_SharedGameInfoLayer->initConnectSprite(true);
}
void GameInfoLayer::removeConnectSprite()
{
    m_SharedGameInfoLayer->initConnectSprite(false);
}
void GameInfoLayer::initConnectSprite(bool addRemove)
{
    if(!addRemove)
    {
        if(!m_spriteConnect)
        {
            m_spriteConnect = Sprite::createWithSpriteFrameName("screen/connected.png");
        }
        else
        {
            m_spriteConnect->stopAllActions();
            m_spriteConnect->setTexture("screen/connected.png");
        }
        auto fadeOut = FadeOut::create(1);
        auto scale = ScaleTo::create(0.2, 1.3);
        auto unScale = ScaleTo::create(0.2, 1);
        auto callFunc = CallFunc::create([=]()
        {
            m_spriteConnect->removeFromParent();
            m_spriteConnect = nullptr;
        });
        auto seq = Sequence::create(scale, unScale, fadeOut, callFunc, NULL);
        m_spriteConnect->runAction(seq);
    }
    else
    {
        if(!m_spriteConnect)
        {
            m_spriteConnect = Sprite::createWithSpriteFrameName("screen/connexion_0.png");
            Vec3 scPos = MainGrid::getPositionXYZ(-1, 0);
            m_spriteConnect->setAnchorPoint(Vec2(0.5, 0.5));
            m_spriteConnect->setPosition(Vec2(scPos.x -20, scPos.y - 36));
            m_infoLayer->addChild(m_spriteConnect);
        }
        
        Vector<SpriteFrame*> animConnect;
        int imageNbr = 5;
        animConnect.reserve(imageNbr);
        auto spriteCache = SpriteFrameCache::getInstance();
        for(int i = 0; i < imageNbr; i++)
        {
            std::string animName = "screen/connexion_" + std::to_string(i) + ".png";
            animConnect.pushBack(spriteCache->getSpriteFrameByName(animName.c_str()));
        }
        
        auto animation = Animation::createWithSpriteFrames(animConnect, 0.5f);
        auto animate = Animate::create(animation);
        
        m_spriteConnect->runAction(RepeatForever::create(animate));
    }
}

//------------------------SOCIAL LOGO------------------------------
void GameInfoLayer::addSocialLogo()
{
    m_SharedGameInfoLayer->initSocialLogo(true);
}
void GameInfoLayer::removeSocialLogo()
{
    m_SharedGameInfoLayer->initSocialLogo(false);
}
void GameInfoLayer::initSocialLogo(bool addRemove)
{
    if(!addRemove)
    {
        if(m_githubLogo)
        {
            m_githubLogo->removeFromParent();
            m_githubLogo = nullptr;
        }
        if(m_twitterLogo)
        {
            m_twitterLogo->removeFromParent();
            m_twitterLogo = nullptr;
        }
    }
    else
    {
        if(!m_githubLogo)
        {
            m_githubLogo = Sprite::createWithSpriteFrameName("screen/github_icon_up.png");
            Vec3 ghPos = MainGrid::getPositionXYZ(-1, 3);
            m_githubLogo->setAnchorPoint(Vec2(0.5, 0.5));
            m_githubLogo->setPosition(Vec2(ghPos.x, ghPos.y));
            m_infoLayer->addChild(m_githubLogo);
        }
        if(!m_twitterLogo)
        {
            m_twitterLogo = Sprite::createWithSpriteFrameName("screen/twitter_icon_up.png");
            Vec3 ttPos = MainGrid::getPositionXYZ(-1, 4);
            m_twitterLogo->setAnchorPoint(Vec2(0.5, 0.5));
            m_twitterLogo->setPosition(Vec2(ttPos.x, ttPos.y));
            m_infoLayer->addChild(m_twitterLogo);
        }
    }
}


//------------------------INFO LOGO------------------------------
void GameInfoLayer::addInfoLogo(std::string name, int line, int collumn)
{
    m_SharedGameInfoLayer->initInfoLogo(name, line, collumn);
}
void GameInfoLayer::initInfoLogo(std::string name, int line, int collumn)
{
    m_fightLogo = Sprite::createWithSpriteFrameName("screen/" + name + "_logo.png");
    Vec3 logoPos = MainGrid::getPositionXYZ(line, collumn);
    Vec2 visibleSize = MainGrid::getVisibleSize();
    m_fightLogo->setPosition(Vec2(logoPos.x, logoPos.y));
    m_fightLogo->setScale(0.3);
    m_infoLayer->addChild(m_fightLogo);
    
    if(m_fightLogo)
    {
        auto scale = ScaleTo::create(1.2, 5);
        auto move = MoveTo::create(1.2, Vec2(visibleSize.x/2, visibleSize.y/2));
        auto fadeOut = FadeOut::create(1);
        auto spawn = Spawn::create(scale, move, fadeOut, NULL);
        auto easeIn = EaseIn::create(spawn, 0.4);
        auto callFunc = CallFunc::create([=]()
        {
            m_fightLogo->removeFromParent();
            m_fightLogo = nullptr;
        });
        auto seq = Sequence::create(easeIn, callFunc, NULL);
        m_fightLogo->runAction(seq);
    }
}

//------------------------EXIT FIGHT BUTTON------------------------------
void GameInfoLayer::addExitFightButton()
{
    m_SharedGameInfoLayer->initExitFightButton(true);
}
void GameInfoLayer::removeExitFightButton()
{
    m_SharedGameInfoLayer->initExitFightButton(false);
}
void GameInfoLayer::initExitFightButton(bool addRemove)
{
    if(!addRemove)
    {
        if(m_exitFightButton)
        {
            m_exitFightButton->removeFromParent();
            m_exitFightButton = nullptr;
        }
    }
    else
    {
        m_exitFightButton = cocos2d::ui::Button::create("screen/exit_fight_button_up.png","screen/exit_fight_button_down.png","screen/exit_fight_button_up.png",cocos2d::ui::TextureResType::PLIST);
        
        m_exitFightButton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
                switch (type)
                {
                        case ui::Widget::TouchEventType::BEGAN:
                                break;
                        case ui::Widget::TouchEventType::ENDED:
                                GameDirector::stopFight(false);
                                break;
                        default:
                                break;
                }
        });
        
        Vec2 buttonSize = m_exitFightButton->getContentSize();
        Vec2 visibleSize = MainGrid::getVisibleSize();
        m_exitFightButton->setPosition(Vec2(visibleSize.x - buttonSize.x/2, visibleSize.y - buttonSize.y/2));
        m_infoLayer->addChild(m_exitFightButton);
    }
}
