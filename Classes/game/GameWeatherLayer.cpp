//
//  GameWeatherLayer.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 21/01/2022.
//

#include "GameWeatherLayer.hpp"

#include "MainObject.hpp"

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
    m_SharedGameWeatherLayer->setCloud(m_SharedGameWeatherLayer->m_cloud_1);
    m_SharedGameWeatherLayer->setCloud(m_SharedGameWeatherLayer->m_cloud_2);
}

bool GameWeatherLayer::setCloud(cocos2d::Sprite* cloud)
{
    std::string cloudFile = "screen/cloud_" + std::to_string(random(0, 4)) + ".png";
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    if(!cloud)
    {
        cloud = cocos2d::Sprite::createWithSpriteFrameName(cloudFile);
        cloud->setAnchorPoint(Vec2(0.5, 0.5));
        cloud->setPosition(Vec2(visibleSize.width, visibleSize.height));
        m_weatherLayer->addChild(cloud);
    }
    else
    {
        cloud->setSpriteFrame(cloudFile);
    }
    
    
    return true;
}
