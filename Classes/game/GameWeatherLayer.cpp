//
//  GameWeatherLayer.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 21/01/2022.
//

#include "GameWeatherLayer.hpp"

#include "MainObject.hpp"
#include "MainGrid.hpp"

USING_NS_CC;

GameWeatherLayer* GameWeatherLayer::getInstance()
{
    if (!m_SharedGameWeatherLayer)
    {
        m_SharedGameWeatherLayer = new (std::nothrow) GameWeatherLayer;
        m_SharedGameWeatherLayer->init();
    }
    return m_SharedGameWeatherLayer;
}

bool GameWeatherLayer::init()
{
    m_weatherLayer = MainObject::getWeatherLayer();
    addClouds();
    
    return true;
}

void GameWeatherLayer::addClouds()
{
    m_SharedGameWeatherLayer->setCloud();
}

void GameWeatherLayer::removeClouds()
{
    auto cloudsList = m_SharedGameWeatherLayer->m_cloudLayer->getChildren();
    
    for(auto cloud : cloudsList)
    {
        if(cloud)
        {
            cloud->removeFromParent();
            cloud = nullptr;
        }
    }
    m_SharedGameWeatherLayer->m_cloudLayer->removeFromParent();
    m_SharedGameWeatherLayer->m_cloudLayer = nullptr;
}

bool GameWeatherLayer::setCloud()
{
    if(!m_cloudLayer)
    {
        m_cloudLayer = cocos2d::Node::create();
        m_weatherLayer->addChild(m_cloudLayer);
    }
    
    std::string cloudFile = "screen/cloud_" + std::to_string(random(0, 4)) + ".png";
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    int visibleWidth = visibleSize.width;
    int visibleHeight = visibleSize.height;
    
    int downField = MainGrid::getPositionXYZ(0, 0).y;
    
    int cloudHeight = random(downField, visibleHeight);
    int cloudRandom = random(0, 128);
    
    auto cloud = cocos2d::Sprite::createWithSpriteFrameName(cloudFile);
    
    cloud->setAnchorPoint(Vec2(0, 0.5));
    cloud->setPosition(Vec2(visibleWidth, cloudHeight));
    cloud->setOpacity(40 + cloudRandom /5);
    cloud->setScale(1 + cloudRandom/100);
    int cloudSpeed = 10 + cloudRandom/5;
    
    m_cloudLayer->addChild(cloud);
    
    auto moveCenter = MoveTo::create(cloudSpeed, Vec2(visibleWidth/2 - cloud->getContentSize().width/2, cloudHeight));
    auto moveOut = MoveTo::create(cloudSpeed, Vec2(-cloud->getContentSize().width, cloudHeight));
    auto centerMoveFunc = CallFunc::create([this]()
    {
        setCloud();
    });
    auto endMoveFunc = CallFunc::create([cloud]()
    {
        cloud->removeFromParent();
    });
    
    auto seq = Sequence::create(moveCenter, centerMoveFunc, moveOut, endMoveFunc, NULL);
    cloud->runAction(seq);
    
    return true;
}
