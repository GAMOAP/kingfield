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
    if(scene == "fight" || scene == "rumble")
    {
        int backLine = line;
        int frontLine = line;
        
        std::string backBreed = breed;
        std::string frontBreed = breed;
        
        bool isFrontBox = false;
        
        if(line>=2)
        {
            backBreed = secondBreed;
        }
        
        if(collumn != -1 && collumn != 5)
        {
            switch (line) {
                case 4:
                    backLine = 0;
                    break;
                case 3:
                    backLine = 1;
                    frontLine = 3;
                    frontBreed = breed;
                    isFrontBox = true;
                    break;
                case 2:
                    backBreed = breed;
                    frontLine = 4;
                    frontBreed = secondBreed;
                    isFrontBox = true;
                    break;
                case 1:
                    frontLine = 3;
                    frontBreed = secondBreed;
                    isFrontBox = true;
                    break;
                    
                default:
                    break;
            }
        }
        
        const std::string backFileName = "box/" + type + "/" + backBreed + "/" + std::to_string(backLine) + "_" + std::to_string(collumn);
        setDisplayBox(m_backBox, backFileName, 0);
        
        if(isFrontBox)
        {
            const std::string frontFileName = "box/" + type + "/" + frontBreed + "/" + std::to_string(frontLine) + "_" + std::to_string(collumn);
            setDisplayBox(m_frontBox, frontFileName, 0);
        }
        
        //printf("FRONT:: %s\n",backFileName.c_str());
    }
    else
    {
        if(m_frontBox)
        {
            m_frontBox->removeFromParent();
            m_frontBox = nullptr;
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
