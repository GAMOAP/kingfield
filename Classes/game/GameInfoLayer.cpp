//
//  GameInfoLayer.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/09/2019.
//

#include "GameInfoLayer.hpp"

#include "MainGrid.hpp"
#include "MainObject.hpp"

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
}
void GameInfoLayer::removeIntroTitle()
{
    m_SharedGameInfoLayer->initIntroTitle(false);
}
void GameInfoLayer::initIntroTitle(bool addRemove)
{
    if(!addRemove)
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
    else
    {
        m_introLogo = Sprite::create(KFSprite::getFile("screen_logo"));
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
            m_spriteConnect = Sprite::create(KFSprite::getFile("screen_connected"));
        }
        else
        {
            m_spriteConnect->stopAllActions();
            m_spriteConnect->setTexture(KFSprite::getFile("screen_connected"));
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
            m_spriteConnect = Sprite::create(KFSprite::getFile("screen_connexion_0"));
            Vec3 scPos = MainGrid::getPositionXYZ(-1, 0);
            m_spriteConnect->setAnchorPoint(Vec2(0.5, 0.5));
            m_spriteConnect->setPosition(Vec2(scPos.x -20, scPos.y - 36));
            m_infoLayer->addChild(m_spriteConnect);
        }
        
        Vector<SpriteFrame*> animConnect;
        int imageNbr = 5;
        animConnect.reserve(imageNbr);
        for(int i = 0; i < imageNbr; i++)
        {
            animConnect.pushBack(SpriteFrame::create(KFSprite::getFile("screen_connexion_" + std::to_string(i)), Rect(0,0,64,64)));
        }
        
        auto animation = Animation::createWithSpriteFrames(animConnect, 0.5f);
        auto animate = Animate::create(animation);
        
        m_spriteConnect->runAction(RepeatForever::create(animate));
    }
}
