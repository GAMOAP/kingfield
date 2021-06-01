//
//  BoxDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/05/2021.
//

#include "BoxDisplay.hpp"

#include "KFSprite.hpp"

USING_NS_CC;

BoxDisplay* BoxDisplay::create(int line, int collumn, std::string type, std::string breed, std::string secondBreed)
{
    BoxDisplay* ret = new (std::nothrow) BoxDisplay();
    if(ret && ret->init (line, collumn, type, breed, secondBreed))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
        return ret;
}

bool BoxDisplay::init(int line, int collumn, std::string type, std::string breed, std::string secondBreed)
{
    const std::string frontFileName = "box_" + type + "_" + secondBreed + "_" + std::to_string(line) + "_" + std::to_string(collumn);
    
    const std::string backFileName = "box_" + type + "_" + breed + "_" + std::to_string(line) + "_" + std::to_string(collumn);
    
    if(!m_frontBox)
    {
        m_frontBox = cocos2d::Sprite::create(KFSprite::getFile(backFileName));
        if(secondBreed == "")
        {
            m_frontBox->setVisible(false);
        }
        m_frontBox->setAnchorPoint(Vec2( 0.5, 0));
        this->addChild(m_frontBox, 1);
    }
    
    if(!m_backBox)
    {
        m_backBox = cocos2d::Sprite::create(KFSprite::getFile(backFileName));
        m_backBox->setAnchorPoint(Vec2( 0.5, 0));
        this->addChild(m_backBox, 0);
    }
    
    return true;
}

void BoxDisplay::setTexture(int line, int collumn, std::string type, std::string breed, std::string secondBreed)
{
    const std::string frontFileName = "box_" + type + "_" + secondBreed + "_" + std::to_string(line) + "_" + std::to_string(collumn);
    
    const std::string backFileName = "box_" + type + "_" + breed + "_" + std::to_string(line) + "_" + std::to_string(collumn);
    
    if(m_frontBox != nullptr)
    {
        m_frontBox->setTexture(KFSprite::getFile(backFileName));
        if(secondBreed == "")
        {
            m_frontBox->setVisible(false);
        }
    }
    
    if(m_backBox != nullptr)
    {
        m_backBox->setTexture(KFSprite::getFile(backFileName));
    }
}

void BoxDisplay::setColor(const cocos2d::Color3B color)
{
    if(m_frontBox != nullptr)
    {
        m_frontBox->setColor(color);
    }
    
    if(m_backBox != nullptr)
    {
        m_backBox->setColor(color);
    }
}
