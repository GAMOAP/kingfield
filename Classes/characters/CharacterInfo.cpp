//
//  CharacterInfo.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 19/03/2021.
//
#include "CharacterInfo.hpp"

#include "KFSprite.hpp"

#include <iostream>

USING_NS_CC;

CharacterInfo* CharacterInfo::create()
{
    CharacterInfo* ret = new (std::nothrow) CharacterInfo();
    if(ret && ret->init ())
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

bool CharacterInfo::init()
{
    
    
    return true;
}

bool CharacterInfo::addToInfoList(std::string infoName, int infoValue)
{
    
    printf("infoName = %s, infoValue = %i\n", infoName.c_str(), infoValue);
    
    for(int u = 0; u < m_typeNameUpList.size(); u++)
    {
        if(infoName == m_typeNameUpList[u])
        {
            const std::vector<int> infoUp= {u,infoValue};
            m_upList.push_back(infoUp);
        }
    }
    for(int p = 0; p < m_typeNamePopList.size(); p++)
    {
        if(infoName == m_typeNamePopList[p])
        {
            const std::vector<int> infoPop = {p,infoValue};
            m_popList.push_back(infoPop);
        }
    }
    displayUp();
    displayPop();
    
    return true;
}

void CharacterInfo::displayUp()
{
    if(m_upList.size() > 0 && !m_upDisplay)
    {
        m_upDisplay = createUpInfo(m_upList[0]);
        this->addChild(m_upDisplay);
        m_upDisplay->setPosition(Vec2(0,64));
        
        const float time = 1;
        auto moveUp = MoveBy::create(time, Vec2(0, 44));
        auto endFunc = CallFunc::create([this]()
        {
            m_upDisplay->removeFromParent();
            m_upDisplay = nullptr;
            m_upList.erase(m_upList.begin());
            displayUp();
        });
        auto seq = Sequence::create(moveUp, endFunc, NULL);
        m_upDisplay->runAction(seq);
    }
}

void CharacterInfo::displayPop()
{
    /*
    float popUpTime = 0.2;
    auto scaleUp = ScaleTo::create(popUpTime, 2.5);
    auto scaleDown = ScaleTo::create(popUpTime, 1);
    auto scaleUpEase = EaseOut::create(scaleUp, 0.5);
    auto scaleDownEase = EaseIn::create(scaleDown, 0.5);
    auto callFunc = CallFunc::create([=]()
    {
        sprite->setName(name);
        sprite->setTexture(KFSprite::getFile(name));
        sprite->setVisible(isVisible);
    });
    auto popUpSeq = Sequence::create(scaleUpEase, callFunc, scaleDownEase, NULL);
    sprite->runAction(popUpSeq);
     */
}

cocos2d::Node* CharacterInfo::createUpInfo(std::vector<int> info)
{
    auto infoDisplay = Node::create();
    
    int infoType = info[0];
    int infoValue = info[1];
    std::string infoFileName = "";
    Vec2 infoPos;
    Color3B infoColor;
    
    bool numbers = false;
    
    switch (infoType) {
        case 0:
            numbers = true;
            infoPos = {0, 0};
            infoColor = {41, 171, 226};
            break;
            
        default:
            break;
    }
    
    if(numbers)
    {
        auto infoSprite = Sprite::create(KFSprite::getFile("charUI_info_number_" + std::to_string(abs(infoValue))));
        infoSprite->setAnchorPoint(Vec2(0,0));
        infoSprite->setPosition(Vec2(0,0));
        infoSprite->setColor(infoColor);
        infoDisplay->addChild(infoSprite);
        
        std::string signString = "plus";
        if(infoValue < 0)
            signString = "minus";
        auto sign = Sprite::create(KFSprite::getFile("charUI_info_number_" + signString));
        sign->setPositionX(-36);
        sign->setColor(infoColor);
        sign->setAnchorPoint(Vec2(0,0));
        infoDisplay->addChild(sign);
    }
    
    return infoDisplay;
}

cocos2d::Node* CharacterInfo::createPopInfo(std::vector<int> info)
{
    auto infoDisplay = Node::create();
    
    return infoDisplay;
}
