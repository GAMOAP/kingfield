//
//  CharacterUI.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//
#include "MainObject.hpp"
#include "MainStuff.hpp"

#include "CharacterUI.hpp"

#include <iostream>

USING_NS_CC;

//STAGE SET..........
CharacterUI* CharacterUI::setCharacterUI(int charNumber)
{
    if (!m_SharedCharacterUI)
    {
        m_SharedCharacterUI = new (std::nothrow) CharacterUI;
    }
    m_SharedCharacterUI->initStage(charNumber);
    
    return m_SharedCharacterUI;
}
void CharacterUI::removeCharacterUI()
{
    if (m_SharedCharacterUI)
    {
        m_SharedCharacterUI->removeFromParent();
        m_SharedCharacterUI->m_underlight->removeFromParent();
        m_SharedCharacterUI = nullptr;
    }
}
bool CharacterUI::initStage(int charNumber)
{
    m_popUpActived = false;
    if(charNumber == m_charNumber)
    {
        m_popUpActived = true;
    }
    
    init(charNumber);
    
    createCrystal();
    createLife();
    createXp();
    createKarma();
    createDefense();
    createAttack();
    createBuff();
    createUnderlight();
    
    if(!m_popUpActived)
        addToStage();
    
    return true;
}

//OPTION SET.......
CharacterUI* CharacterUI::getOptionCharacterUI(int charNumber)
{
    if(!m_SharedOptionCharacterUI)
    {
        m_SharedOptionCharacterUI = new (std::nothrow) CharacterUI;
    }
    m_SharedOptionCharacterUI->initOption(charNumber);
    
    return m_SharedOptionCharacterUI;
}

bool CharacterUI::initOption(int charNumber)
{
    m_popUpActived = false;
    
    init(charNumber);
    
    createCrystal();
    createLife();
    createXp();
    createKarma();
    createDefense();
    createAttack();
    createBuff();
    
    return true;
}

//init...........
bool CharacterUI::init(int charNumber)
{
    setName("charUI");
    
    auto mainStuff = MainStuff::getInstance();
    
    m_charNumber = charNumber;
    
    m_charSpec = mainStuff->getCharSpec(charNumber);
    m_karmaBreed = mainStuff->getKarma(charNumber);
    
    return true;
}

//Crystal
void CharacterUI::createCrystal()
{
    if(m_crystalList.size() <= 0)
    {
        for(int c = 0; c < m_crystalNbr; c++)
        {
            auto crystalPointer = Sprite::createWithSpriteFrameName("UI/char/crystalred.png");
            crystalPointer->setName("crystal");
            int decY = 0;
            if(c%2 == 0){decY = 10;}
            crystalPointer->setAnchorPoint(Vec2( 0.5, 0.5));
            crystalPointer->setPosition(Vec2(m_crystalListX + c*10, m_crystalListY + decY));
            crystalPointer->setVisible(false);
            m_crystalList.push_back(crystalPointer);
            this->addChild(crystalPointer, 2);
        }
    }
    
    for(int c = 0; c < m_crystalList.size(); c++)
    {
        auto crystalPointer = m_crystalList[c];
        
        std::string isVoid = "";
        if(c >= m_charSpec["crystal"])
            isVoid = "void";
            
        if(c < m_charSpec["crystal_red"])
        {
            popUp(crystalPointer, "UI/char/crystal" + isVoid + "red.png",true);
        }
        else if(c < m_charSpec["crystal_red"] + m_charSpec["crystal_blue"])
        {
            popUp(crystalPointer, "UI/char/crystal" + isVoid + "blue.png",true);
        }
        else
        {
            popUp(crystalPointer, "UI/char/crystalred.png",false);
        }
    }
}

//Life
void CharacterUI::createLife()
{
    if(m_lifeList.size() <= 0)
    {
        for(int l = 0; l < m_lifeNbr; l++)
        {
            auto lifePointer = Sprite::createWithSpriteFrameName("UI/char/lifebar.png");
            lifePointer->setAnchorPoint(Vec2( 0.5, 0.5));
            lifePointer->setPosition(Vec2(m_lifeListX + l*11, m_lifeListY));
            lifePointer->setVisible(false);
            m_lifeList.push_back(lifePointer);
            this->addChild(lifePointer, 1);
        }
    }
    for(int l = 0; l < m_lifeList.size(); l++)
    {
        auto lifePointer = m_lifeList[l];
        
        std::string isVoid = "";
        if(l >= m_charSpec["health"])
            isVoid = "void";
        
        if(l < m_charSpec["life"])
            popUp(lifePointer, "UI/char/lifebar" + isVoid + ".png" , true);
        else
            popUp(lifePointer, "UI/char/lifebar.png", false);
    }
}

//XP
void CharacterUI::createXp()
{
    if(m_xpList.size() <= 0)
    {
        for(int x = 0; x < m_xpNbr; x++)
        {
            auto xpPointer = Sprite::createWithSpriteFrameName("UI/char/xpbar.png");
            xpPointer->setAnchorPoint(Vec2( 0, 0));
            xpPointer->setPosition(Vec2(m_xpListX + x*4, m_xpListY));
            xpPointer->setVisible(false);
            m_xpList.push_back(xpPointer);
            this->addChild(xpPointer, 0);
        }
    }
    
    int xp = m_charSpec["xp"];
    int level = m_charSpec["level_xp"];
    int level_xp = LEVELS[level];
    printf("manageXp::---->character_%i xp = %i, level = %i level_xp = %i\n", m_charNumber, xp, level, level_xp);
    
    for(int x = 0; x < m_xpList.size(); x++)
    {
        auto xpPointer = m_xpList[x];
        
        std::string isVoid = "";
        std::string isEndBar = "";
        if(x == level_xp -1)
        {
            isEndBar  = "_end";
        }
        
        if(x >= xp)
        {
            isVoid = "_void";
        }
        
        if(x < level_xp)
        {
            popUp(xpPointer, "UI/char/xpbar" + isVoid + isEndBar + ".png" , true);
        }
        else
        {
            popUp(xpPointer, "UI/char/xpbar.png", false);
        }
    }
}

//Karma
void CharacterUI::createKarma()
{
    if(!m_karma)
    {
        m_karma = Sprite::createWithSpriteFrameName("UI/char/karma_" + m_karmaBreed + ".png");
        m_karma->setName("UI/char/karma_" + m_karmaBreed + ".png");
        m_karma->setAnchorPoint(Vec2( 0.5, 0.5));
        m_karma->setPosition(m_karmaPosition);
        this->addChild(m_karma, 0);
    }
    else
    {
        popUp(m_karma, "UI/char/karma_" + m_karmaBreed + ".png");
    }
}

//Defense
void CharacterUI::createDefense()
{
    std::string specName = "shield";
    
    //buffed or unbuffed defense
    int specDefense = m_charSpec["defense"];
    int specShield = m_charSpec["shield"];
    
    std::string specBuffedName = "defense";
    std::string attributeName = "front";
    cocos2d::Color3B buffedColor = Color3B::WHITE;
    
    if(specShield > specDefense)
    {
        specName = "defense";
        specBuffedName = "shield";
        attributeName = "background";
        buffedColor = Color3B::GREEN;
    }
    
    if(!m_defense)
    {
        m_defense = Node::create();
        m_defense->setName("UI/char/defense_" + std:: to_string(m_charSpec[specName]) + ".png");
        m_defense->setAnchorPoint(Vec2( 0.5, 0.5));
        m_defense->setPosition(m_defensePosition);
        this->addChild(m_defense);
        
        m_defense_picture = Sprite::createWithSpriteFrameName("UI/char/defense_picture.png");
        m_defense->addChild(m_defense_picture, 0);
        
        m_defense_value = Sprite::create();
        if(m_charSpec[specName] > 0)
        {
            m_defense_value->setSpriteFrame("UI/char/attributes_background_" + std:: to_string(m_charSpec[specName]) + ".png");
        }
        m_defense_value->setColor(m_colorDefense);
        m_defense->addChild(m_defense_value, 1);
    }
    else
    {
        if(m_defense_value->getName() != "UI/char/attributes_background_" + std:: to_string(m_charSpec[specName]) + ".png")
        {
            popUp(m_defense_picture, "UI/char/defense_picture.png");
        }
        if(m_charSpec[specName] > 0)
        {
            popUp(m_defense_value, "UI/char/attributes_background_" + std:: to_string(m_charSpec[specName]) + ".png");
        }
    }
    
    if(specShield != specDefense)
    {
        if(!m_defense_buff)
        {
            m_defense_buff = Sprite::createWithSpriteFrameName("UI/char/attributes_" + attributeName + "_" + std:: to_string(m_charSpec[specBuffedName]) + ".png");
            m_defense_buff->setColor(buffedColor);
            m_defense->addChild(m_defense_buff, 0);
        }
        else
        {
            m_defense_buff->setSpriteFrame("UI/char/attributes_" + attributeName + "_" + std:: to_string(m_charSpec[specBuffedName]) + ".png");
            m_defense_buff->setColor(buffedColor);
        }
    }
    else
    {
        if(m_defense_buff)
        {
            m_defense_buff->removeFromParent();
            m_defense_buff = nullptr;
        }
    }
}

//Attack
void CharacterUI::createAttack()
{
    std::string specName = "force";
    
    //buffed or unbuffed attack
    int specAttack = m_charSpec["attack"];
    int specForce = m_charSpec["force"];
    
    std::string specBuffedName = "attack";
    std::string attributeName = "front";
    cocos2d::Color3B buffedColor = Color3B::WHITE;
    
    if(specForce > specAttack)
    {
        specName = "attack";
        specBuffedName = "force";
        attributeName = "background";
        buffedColor = Color3B::GREEN;
    }
    
    if(!m_attack)
    {
        m_attack = Node::create();
        m_attack->setName("UI/char/attack_" + std:: to_string(m_charSpec[specName]) + ".png");
        m_attack->setAnchorPoint(Vec2( 0.5, 0.5));
        m_attack->setPosition(m_attackPosition);
        this->addChild(m_attack);
        
        m_attack_picture = Sprite::createWithSpriteFrameName("UI/char/attack_picture.png");
        m_attack->addChild(m_attack_picture, 0);
        
        m_attack_value = Sprite::create();
        if(m_charSpec[specName] > 0)
        {
            m_attack_value->setSpriteFrame("UI/char/attributes_background_" + std:: to_string(m_charSpec[specName]) + ".png");
        }
        m_attack_value->setColor(m_colorAttack);
        m_attack->addChild(m_attack_value, 1);
    }
    else
    {
        if(m_attack_value->getName() != "UI/char/attributes_background_" + std:: to_string(m_charSpec[specName]) + ".png")
        {
            popUp(m_attack_picture, "UI/char/attack_picture.png");
        }
        popUp(m_attack_value, "UI/char/attributes_background_" + std:: to_string(m_charSpec[specName]) + ".png");
    }
    
    if(specForce != specAttack)
    {
        if(!m_attack_buff)
        {
            m_attack_buff = Sprite::createWithSpriteFrameName("UI/char/attributes_" + attributeName + "_" + std:: to_string(m_charSpec[specBuffedName]) + ".png");
            m_attack_buff->setColor(buffedColor);
            m_attack->addChild(m_attack_buff, 0);
        }
        else
        {
            m_attack_buff->setSpriteFrame("UI/char/attributes_" + attributeName + "_" + std:: to_string(m_charSpec[specBuffedName]) + ".png");
            m_attack_buff->setColor(buffedColor);
        }
    }
    else
    {
        if(m_attack_buff)
        {
            m_attack_buff->removeFromParent();
            m_attack_buff = nullptr;
        }
    }
}

//Buff
void CharacterUI::createBuff()
{
    auto character = MainObject::getCharByNumber(m_charNumber);
    std::string buffName = character->getBuffName();
    int turnLeft = character->getBuffTurnLeft();
    
    if(buffName != "")
    {
        if(!m_buff)
        {
            m_buff = Node::create();
            m_buff_picture = Sprite::createWithSpriteFrameName("UI/char/" + buffName + "_picture.png");
            m_buff_picture->setAnchorPoint(Vec2(0.5, 0.5));
            m_buff_picture->setPosition(m_buffPosition);
            m_buff_picture->setScale(0.75);
            m_buff->addChild(m_buff_picture, 0);
            
            m_buff_turnLeft = Sprite::createWithSpriteFrameName("UI/char/buff_turn_left_" + std::to_string(turnLeft) + ".png");
            m_buff_turnLeft->setAnchorPoint(Vec2(0.5, 0.5));
            m_buff_turnLeft->setPosition(m_buffTurnLeftPosition);
            m_buff->addChild(m_buff_turnLeft, 1);
            this->addChild(m_buff);
            m_buff->setVisible(true);
        }
        else
        {
            m_buff_picture->setSpriteFrame("UI/char/" + buffName + "_picture.png");
            m_buff_turnLeft->setSpriteFrame("UI/char/buff_turn_left_" + std::to_string(turnLeft) + ".png");
            m_buff->setVisible(true);
        }
    }
    else
    {
        if(m_buff)
        {
            m_buff->setVisible(false);
        }
    }
}

//Underlight
void CharacterUI::createUnderlight()
{
    if(!m_underlight)
    {
        m_underlight = Sprite::createWithSpriteFrameName("UI/char/underlight_" + m_karmaBreed + ".png");
    }
    else
    {
        m_underlight->setSpriteFrame("UI/char/underlight_" + m_karmaBreed + ".png");
    }
}

//add to stage.
void CharacterUI::addToStage()
{
    auto character = MainObject::getCharByNumber(m_charNumber);
    if(character)
    {
        this->retain();
        this->removeFromParent();
        character->addChild(this, m_indexUI);
        this->release();
        
        auto box = MainObject::getBoxByTag(character->getTag());
        if(box)
        {
            m_underlight->retain();
            m_underlight->removeFromParent();
            box->addChild(m_underlight, m_indexUnderlight);
            m_underlight->release();
        }
        if(m_charNumber >= 5)
        {
            this->setScaleX(-1);
        }
        else
        {
            this->setScaleX(1);
        }
    }
}

//pop up
void CharacterUI::popUp(Sprite* sprite, std::string name, bool isVisible)
{
    
    name.find("picture");
    
    if(!m_popUpActived)
    {
        sprite->setName(name);
        sprite->setSpriteFrame(name);
        sprite->setVisible(isVisible);
    }
    else if(sprite->getName() != name || sprite->isVisible() != isVisible || name.find("picture") != std::string::npos)
    {
        float popUpTime = 0.2;
        auto scaleUp = ScaleTo::create(popUpTime, 2.5);
        auto scaleDown = ScaleTo::create(popUpTime, 1);
        auto scaleUpEase = EaseOut::create(scaleUp, 0.5);
        auto scaleDownEase = EaseIn::create(scaleDown, 0.5);
        auto callFunc = CallFunc::create([=]()
        {
            sprite->setName(name);
            sprite->setSpriteFrame(name);
            sprite->setVisible(isVisible);
        });
        auto popUpSeq = Sequence::create(scaleUpEase, callFunc, scaleDownEase, NULL);
        sprite->runAction(popUpSeq);
    }
}
