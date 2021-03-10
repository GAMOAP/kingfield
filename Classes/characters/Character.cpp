//
//  Character.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//
#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainAction.hpp"

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
    
    if(m_originTagList[number] % 5 == 0)
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
    
    //init action.
    int actionType = action->getType();
    
    //move character...
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
}
//------------------INFO----------------
void Character::setInfo(std::string infoName, int infoValue)
{
    m_characterDisplay->setInfo(infoName, infoValue);
}

//------------------SELECT----------------------
void Character::setSelect()
{
    m_select = true;
    m_characterDisplay->setSelect();
    this->setLocalZOrder(32);
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

