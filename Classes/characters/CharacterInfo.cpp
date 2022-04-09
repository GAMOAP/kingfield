//
//  CharacterInfo.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 19/03/2021.
//
#include "CharacterInfo.hpp"

#include "MainStuff.hpp"

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

bool CharacterInfo::addToInfoList(int characterNbr, std::string infoName, int infoValue)
{
    m_characterNbr = characterNbr;
    
    for(int u = 0; u < m_typeNameUpList.size(); u++)
    {
        if(infoName == m_typeNameUpList[u])
        {
            printf("CharacterInfo::  infoName = %s , m_typeNameUpList[u] = %s \n", infoName.c_str(), m_typeNameUpList[u].c_str());
            
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
        
        const float time = 0.5;
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
    if(m_popList.size() > 0 && !m_popDisplay)
    {
        m_popDisplay = createPopInfo(m_popList[0]);
        this->addChild(m_popDisplay);
        m_popDisplay->setAnchorPoint(Vec2( 0.5, 0.5));
        
        Vec2 popPosition = Vec2{0, -38};
        
        switch (m_popList[0][0]) {
            case 0:
                popPosition = m_defensePosition;
                break;
            case 1:
                popPosition = m_attackPosition;
                break;
            default:
                break;
        }
        m_popDisplay->setPosition(popPosition);
        
        float popUpTime = 0.2;
        auto scaleUp = ScaleTo::create(popUpTime, 2.5);
        auto scaleDown = ScaleTo::create(popUpTime/2, 1);
        auto scaleUpEase = EaseOut::create(scaleUp, 0.5);
        auto scaleDownEase = EaseIn::create(scaleDown, 0.5);
        auto callFunc = CallFunc::create([=]()
        {
            m_popDisplay->removeFromParent();
            m_popDisplay = nullptr;
            m_popList.erase(m_popList.begin());
            displayPop();
        });
        auto popUpSeq = Sequence::create(scaleUpEase, scaleDownEase, callFunc, NULL);
        m_popDisplay->runAction(popUpSeq);
    }
}

cocos2d::Node* CharacterInfo::createUpInfo(std::vector<int> info)
{
    auto infoDisplay = Node::create();
    
    int infoType = info[0];
    int infoValue = info[1];
    std::string infoFileName = m_typeNameUpList[infoType];
    Vec2 infoPos;
    Color3B infoColor;
    
    bool numbers = false;
    
    switch (infoType) {
        case 0://crystal
            numbers = true;
            infoPos = {0, 0};
            break;
        case 1://health
            numbers = true;
            infoPos = {0, 0};
            break;
        case 2://xp
            numbers = true;
            infoPos = {0, 0};
            break;
        case 3://level
            numbers = true;
            infoPos = {0, 0};
            break;
            
        default:
            break;
    }
    auto picture = Sprite::createWithSpriteFrameName("UI/char/info_" + infoFileName + ".png");
    picture->setAnchorPoint(Vec2(0.5,0));
    infoDisplay->addChild(picture, 0);
    
    if(numbers)
    {
        std::string signString = "plus";
        infoColor = {41, 171, 226};
        if(infoValue < 0)
        {
            signString = "minus";
            infoColor = {235, 34, 39};
        }
        
        auto digit = Sprite::createWithSpriteFrameName("UI/char/info_number_" + std::to_string(abs(infoValue)) + ".png");
        digit->setAnchorPoint(Vec2(0,0));
        digit->setPosition(Vec2(0,0));
        digit->setColor(infoColor);
        infoDisplay->addChild(digit);
        
        auto sign = Sprite::createWithSpriteFrameName("UI/char/info_number_" + signString + ".png");
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
    
    std::string infoPopName = m_typeNamePopList[info[0]];
    int infoPopValue = info[1];
    
    //add picture
    m_pop_picture = Sprite::createWithSpriteFrameName("UI/char/" + infoPopName + "_picture.png");
    infoDisplay->addChild(m_pop_picture);
    
    if(infoPopValue >= 0)
    {
        cocos2d::Color3B color = m_colorDefense;
        if(infoPopName == "attack")
        {
            color = m_colorAttack;
        }
        
        int charSpecTotal = MainStuff::getCharSpec(m_characterNbr)[infoPopName];
        
        //add background
        m_popValueTotalBackgroud = Sprite::createWithSpriteFrameName("UI/char/attributes_background_" + std::to_string(charSpecTotal) + ".png");
        infoDisplay->addChild(m_popValueTotalBackgroud);
        
        //add value
        if(infoPopValue > 0)
        {
            m_popValue = Sprite::createWithSpriteFrameName("UI/char/attributes_background_" + std::to_string(infoPopValue) + ".png");
            m_popValue->setColor(color);
            infoDisplay->addChild(m_popValue);
        }
        
        //add front
        m_popValueTotalfront = Sprite::createWithSpriteFrameName("UI/char/attributes_front_" + std:: to_string(charSpecTotal) + ".png");
        m_popValueTotalfront->setColor(color);
        infoDisplay->addChild(m_popValueTotalfront);
    }
    
    return infoDisplay;
}
