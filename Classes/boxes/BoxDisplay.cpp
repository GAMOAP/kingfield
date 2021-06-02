//
//  BoxDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/05/2021.
//

#include "BoxDisplay.hpp"

#include "KFSprite.hpp"

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
    if(scene == "fight")
    {
        if(line >= 2)
        {
            if(line >= 0 && line <= 4 && collumn != -1 && collumn != 5)
            {
                if(line == 4)
                {
                    line = 0;
                }
                if(line == 3)
                {
                    line = 1;
                }
            }
            breed = secondBreed;
        }
        
        const std::string fileName = "box_" + type + "_" + breed + "_" + std::to_string(line) + "_" + std::to_string(collumn);
        
        printf("SET:: %s\n",fileName.c_str());
        
        setDisplayBox(m_backBox, fileName, 0);
    }
    else
    {
        if(m_frontBox)
        {
            m_frontBox->removeFromParent();
            m_frontBox = nullptr;
        }
        
        const std::string fileName = "box_" + type + "_" + breed + "_" + std::to_string(line) + "_" + std::to_string(collumn);
        
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
        box = cocos2d::Sprite::create(KFSprite::getFile(fileName));
        box->setAnchorPoint(Vec2( 0.5, 0));
        this->addChild(box, index);
    }
    else
    {
        box->setTexture(KFSprite::getFile(fileName));
    }
    
    return box;
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
