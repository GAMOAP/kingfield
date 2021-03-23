//
//  Character.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//
#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainAction.hpp"
#include "MainStuff.hpp"

#include "Character.hpp"

#include "CharacterUI.hpp"

#include <iostream>

USING_NS_CC;

Character* Character::setCharacter(int number)
{
    auto character = MainObject::getCharByNumber(number);
    if(!character)
    {
        Character* ret = new (std::nothrow) Character();
        if(ret && ret->init(number))
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
    }
    else
    {
        character->initDisplay();
        return character;
    }
}

bool Character::init(int number)
{
    //Create identity.
    setNumber(number);
    setTag(m_originTagList[number]);
    Vec2 positionLC = MainGrid::getLineCollumnByTag(_tag);
    
    m_className = "char";
    setName(m_className + "_" + std::to_string(number));
    
    m_line = positionLC.x;
    m_collumn = positionLC.y;
    
    if(m_originTagList[number] >= 60)
    {
        this->setFlip();
        m_isPlayerTeam = false;
    }
    else
    {
        m_isPlayerTeam = true;
    }
    
    addToStage();
    
    remove();
    auto IsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
    {
        initDisplay();
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(IsOutEvent, this);
    
    return true;
}

void Character::initDisplay()
{
    if(!m_characterDisplay)
    {
        m_characterDisplay = CharacterDisplay::create(m_number);
        this->addChild(m_characterDisplay, 1);
    }
}

bool Character::removeToStage()
{
    remove();
    auto boxIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
    {
        m_characterDisplay->stopAnimation();
        if(m_SharedCharacterUI){m_SharedCharacterUI = nullptr;}
        this->removeFromParent();
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(boxIsOutEvent, this);
    return true;
}

void Character::setStuff()
{
    m_characterDisplay->setStuff();
}

//---------------------------ACTION--------------------------
void Character::setAction(KFAction* action)
{
    auto mainGrid = MainGrid::getInstance();
    auto mainStuff = MainStuff::getInstance();
    
    //init action.
    int actionType = action->getType();
    
    //move character...
    setIsMove(true);
    
    if(actionType == 0)
    {
        int startTag = action->getSartTag();
        int endTag = action->getEndTag();
        const Vec2 positionLC = mainGrid->getLineCollumnByTag(endTag);
        const int line = positionLC.x;
        const int collumn = positionLC.y;
        
        const Vec3 positionXYZ = mainGrid->getPositionXYZ(line, collumn);
        
        float decTime = 0;
        if(line != m_line && collumn != m_collumn){decTime = 0.3;}
        if(line < m_line){setLocalZOrder(positionXYZ.z + getIndexClassName() - 1);}
        
        auto move = MoveTo::create(m_actionTime + decTime, Vec2(positionXYZ.x, positionXYZ.y));
        
        m_line = line;
        m_collumn = collumn;
        
        auto endFunc = CallFunc::create([this]()
        {
            setNodePosition();
            m_characterDisplay->setAction("stand");
            _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
        });
        auto seq = Sequence::create(move, endFunc, NULL);
        this->runAction(seq);
        
        m_characterDisplay->setAction("walk");
    }
    //character strike...
    if(actionType == 1)
    {
        m_characterDisplay->setAction("attack_" + mainStuff->getStuffByName(m_number, 2)[1], 1);
        const int strikeAttack = action->getCharAttackForce();
        setInfo("attack", strikeAttack);
        
        std::vector<std::vector<int>> strikedList = action->getCharStrikedList();
        
        auto delayStrike = DelayTime::create(0.85);
        auto strikeFunc = CallFunc::create([this, strikedList, strikeAttack]()
        {
            for(int c = 0; c < strikedList.size(); c++)
            {
                const auto strikedChar = MainObject::getCharByNumber(strikedList[c][0]);
                const int strikedDefence = strikedList[c][1];
                if(strikedChar)
                {
                    strikedChar->setInfo("defense", strikedDefence);
                    if(strikeAttack >= strikedDefence)
                        strikedChar->setReaction("pain");
                    else
                        strikedChar->setReaction("block");
                }
            }
        });
        auto delayEnd = DelayTime::create(0.3);
        auto endFunc = CallFunc::create([this]()
        {
            _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
        });
        auto seq = Sequence::create(delayStrike, strikeFunc, delayEnd, endFunc, NULL);
        this->runAction(seq);
    }
}

void Character::setReaction(std::string reaction)
{
    if(reaction == "pain")
    {
        MainStuff::setCharSpec(m_number, "health", -1);
        m_characterDisplay->setAction("pain", 1);
    }
    if(reaction == "block")
    {
        m_characterDisplay->setAction("block", 1);
    }
}
//------------------INFO----------------
void Character::setInfo(std::string infoName, int infoValue)
{
    m_characterDisplay->setInfo(infoName, infoValue);
}

//------------------SELECT----------------------
void Character::setSelect(bool isTurn)
{
    if(isTurn)
    {
        m_select = true;
        m_characterDisplay->setSelect();
        this->setLocalZOrder(32);
    }
    else
    {
        this->setLocalZOrder(32);
    }
}

void Character::setUnselect()
{
    m_select = false;
    m_characterDisplay->setUnselect();
    this->setLocalZOrder(m_index);
}
//-----------------KING-------------------------
void Character::setFlag()
{
    m_characterDisplay->setFlag();
}

//-----------------TEAM-------------------------
bool Character::getIsPlayerTeam()
{
    return m_isPlayerTeam;
}

