//
//  BoxFieldDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 10/01/2022.
//

#include "BoxFieldDisplay.hpp"

USING_NS_CC;

BoxFieldDisplay* BoxFieldDisplay::create(int line, int collumn, std::string breed, std::string secondBreed)
{
    BoxFieldDisplay* ret = new (std::nothrow) BoxFieldDisplay();
    if(ret && ret->init (line, collumn, breed, secondBreed))
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

void BoxFieldDisplay::setTexture(int line, int collumn, std::string breed, std::string secondBreed)
{
    setBreedArray(line);
    
    //back box
    const std::string backBoxFileName = "box/field/" + breed + "/B_" + std::to_string(line) + "_" + secondBreed;
    if(!m_backBox)
    {
        m_backBox = cocos2d::Sprite::createWithSpriteFrameName(backBoxFileName + ".png");
        m_backBox->setAnchorPoint(Vec2( 0.5, 0));
        this->addChild(m_backBox, 0);
    }
    else
    {
        m_backBox->setSpriteFrame(backBoxFileName + ".png");
    }
    
    //mask box
    if(!m_maskBox)
    {
        m_maskBox = cocos2d::Sprite::createWithSpriteFrameName("box/effect/mask.png");
        
        const unsigned char r = 10 *random(0,10) + ((line + collumn)%2) * 155;
        const cocos2d::Color3B grayValue = {r, r, r};
        m_maskBox->setColor(grayValue);
        m_maskBox->setOpacity(20);
        
        m_maskBox->setAnchorPoint(Vec2( 0.5, 0));
        this->addChild(m_maskBox, 1);
        
        
    }
    
    //effect box
    if(!m_effectBox)
    {
        m_effectBox = cocos2d::Sprite::createWithSpriteFrameName("box/effect/shadow.png");
        m_effectBox->setAnchorPoint(Vec2( 0.5, 0));
        this->addChild(m_effectBox, 2);
    }
    
    if(!m_gridBox)
    {
        std::string l = "c";
        std::string c = "c";
        if(line == 0)
        {
            l = "d";
        }
        if(line == 4)
        {
            l = "h";
        }
        if(collumn == 0)
        {
            c = "l";
        }
        if(collumn == 4)
        {
            c = "r";
        }
        
        if(l != "c" || c != "c")
        {
            m_gridBox = cocos2d::Sprite::createWithSpriteFrameName("box/effect/grid_" + l + "_" + c + ".png");
            m_gridBox->setAnchorPoint(Vec2( 0.5, 0));
            this->addChild(m_gridBox, 3);
        }
        
        
    }
    
    //mini box
    for(int s = 0; s < 6; s++)
    {
        std::string boxBreed = breed;
        if(line == 1 )
        {
            boxBreed = secondBreed;
            if(m_breedArray[s] == false)
            {
                boxBreed = breed;
            }
        }
        else
        {
            if(m_breedArray[s] == false)
            {
                boxBreed = secondBreed;
            }
        }
        
        const std::string miniBoxFileName = "box/field/" + boxBreed + "/A_" + std::to_string(random(0, 5));
        
        cocos2d::Sprite* miniBox = m_miniBoxArray[s];
        if(!miniBox)
        {
            miniBox = cocos2d::Sprite::createWithSpriteFrameName(miniBoxFileName + ".png");
            miniBox->setAnchorPoint(Vec2( 0, 0));
            
            int w = s % 2 - 1;
            int h = s/2;
            
            miniBox->setPosition(Vec2(miniBox->getContentSize().width * w, 168 + (miniBox->getContentSize().height - 4) * h));
            this->addChild(miniBox, 10 - s);
            m_miniBoxArray[s] = miniBox;
        }
        else
        {
            miniBox->setSpriteFrame(miniBoxFileName + ".png");
        }
    }
}

BoxFieldDisplay:: ~BoxFieldDisplay()
{
    for(int s = 0; s < 6; s++)
    {
        cocos2d::Sprite* miniBox = m_miniBoxArray[s];
        if(miniBox)
        {
            miniBox->removeFromParent();
            miniBox = nullptr;
        }
    }
}

bool BoxFieldDisplay::init(int line, int collumn, std::string breed, std::string secondBreed)
{
    setTexture(line, collumn, breed, secondBreed);
    
    return true;
}

void BoxFieldDisplay::setBreedArray(int line)
{
    int r;
    if(line == 2)
    {
        r = random(0,17);
    }
    else if (line == 1 || line == 3)
    {
        r = random(0,6);
    }
    
    for(int a = 0; a < 6; a++)
    {
        if(line == 2)
        {
            m_breedArray[a] = m_bitArray3[r][a];
        }
        else if (line == 1 || line == 3)
        {
            m_breedArray[a] = m_bitArray2[r][a];
        }
        else
        {
            m_breedArray[a] = 0;
        }
    }
}

void BoxFieldDisplay::setBoxColor(const cocos2d::Color3B color)
{
    Color3B colorTested = {255, 255, 255};
    
    m_maskBox->setVisible(true);
    if(colorTested == color)
    {
        m_maskBox->setVisible(false);
    }
    
    if(m_backBox)
    {
        m_backBox->setColor(color);
    }
    
    for(int a = 0; a < 6; a++)
    {
        cocos2d::Sprite* miniBox = m_miniBoxArray[a];
        if(miniBox)
        {
            miniBox->setColor(color);
        }
    }
}
