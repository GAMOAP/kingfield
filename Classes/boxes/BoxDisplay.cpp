//
//  BoxDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/05/2021.
//

#include "BoxDisplay.hpp"

USING_NS_CC;

BoxDisplay* BoxDisplay::create(int line, int collumn, std::string type, std::string breed, std::string secondBreed, std::string scene)
{
    BoxDisplay* ret = new (std::nothrow) BoxDisplay();
    if(ret && ret->init (line, collumn, type, breed, secondBreed, scene))
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

bool BoxDisplay::init(int line, int collumn, std::string type, std::string breed, std::string secondBreed, std::string scene)
{
    if((scene == "fight" || scene == "rumble" || (scene == "barrack" && type == "field")) &&
       line >= 0 && line <= 4 &&
       collumn >= 0 && collumn <= 4)
    {
        if(m_backBox)
        {
            m_backBox->removeFromParent();
            m_backBox = nullptr;
        }
        
        if(!m_fieldBox)
        {
            m_fieldBox = BoxFieldDisplay::create(line, collumn, breed, secondBreed);
            this->addChild(m_fieldBox, 1);
        }
        else
        {
            m_fieldBox->setTexture(line, collumn, breed, secondBreed);
        }
    }
    else
    {
        if(m_fieldBox)
        {
            m_fieldBox->removeFromParent();
            m_fieldBox = nullptr;
        }
        
        const std::string fileName = "box/" + type + "/" + breed + "/" + std::to_string(line) + "_" + std::to_string(collumn);
        
        m_backBox = setDisplayBox(m_backBox, fileName, 0);
    }
    
    return true;
}

void BoxDisplay::setTexture(int line, int collumn, std::string type, std::string breed, std::string secondBreed, std::string scene)
{
    init(line, collumn, type, breed, secondBreed, scene);
}

cocos2d::Sprite* BoxDisplay::setDisplayBox(Sprite* box, std::string fileName, int index)
{
    if(!box)
    {
        box = cocos2d::Sprite::createWithSpriteFrameName(fileName + ".png");
        box->setAnchorPoint(Vec2( 0.5, 0));
        this->addChild(box, index);
    }
    else
    {
        box->setSpriteFrame(fileName + ".png");
    }
    
    return box;
}

void BoxDisplay::setBoxColor(const cocos2d::Color3B color)
{
    if(m_fieldBox != nullptr)
    {
        m_fieldBox->setBoxColor(color);
    }
    
    if(m_backBox != nullptr)
    {
        m_backBox->setColor(color);
    }
}
