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
bool Character::setAction(KFAction* action)
{
    auto mainStuff = MainStuff::getInstance();
    
    //init action.
    int actionType = action->getType();
    
    //move character...
    setIsMove(true);
    
    if(actionType == 0)
    {
        int endTag = action->getEndTag();
        setMove(endTag);
    }
    //character strike...
    if(actionType == 1)
    {
        const std::string actionSlotType = action->getSlotSpec();
        std::vector<std::vector<int>> strikedList = action->getCharStrikedList();
        const int force = action->getCharAttackForce();
        if(force <=0)
        {
            printf("fail");
            m_characterDisplay->setAnimation("fail", 1);
            auto delayFail = DelayTime::create(1);
            auto failFunc = CallFunc::create([this]()
            {
                _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
            });
            auto seq = Sequence::create(delayFail, failFunc, NULL);
            this->runAction(seq);
            return false;
        }
        
        if(actionSlotType == "strike")
        {
            strikeChar(strikedList, force);
        }
        if(actionSlotType == "heal")
        {
            healChar(strikedList);
        }
    }
    return true;
}



//------------------ACTION MOVE----------------
bool Character::setMove(int endTag)
{
    auto mainGrid = MainGrid::getInstance();
    
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
        m_characterDisplay->setAnimation("stand");
        _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
    });
    auto seq = Sequence::create(move, endFunc, NULL);
    this->runAction(seq);
    
    m_characterDisplay->setAnimation("walk");
    
    return true;
}
//------------------ACTION STRIKE CHAR----------------
bool Character::strikeChar(std::vector<std::vector<int>> strikedList, int force)
{
    auto mainStuff = MainStuff::getInstance();
    
    this->setLocalZOrder(32);
    m_characterDisplay->setAnimation("attack_" + mainStuff->getStuffByName(m_number, 2)[1], 1);
    setInfo("attack", force);
    
    auto delayStrike = DelayTime::create(1);
    auto strikeFunc = CallFunc::create([this, strikedList, force]()
    {
        this->setLocalZOrder(m_index);
        for(int c = 0; c < strikedList.size(); c++)
        {
            //strikeChar
            const auto strikedChar = MainObject::getCharByNumber(strikedList[c][0]);
            const int health = MainStuff::getCharSpec(strikedList[c][0])["health"];
            const int defence = strikedList[c][1];
            
            if(strikedChar)
            {
                strikedChar->setLocalZOrder(32);
                
                strikedChar->setInfo("defense", defence);
                
                m_reaction reactionName = block;
                if(force >= defence)
                {
                    if(health <= 1)
                    {
                        reactionName = death;
                    }
                    else
                    {
                        reactionName = pain;
                    }
                }
                
                std::string endEventName = strikedChar->setReaction(reactionName);
                auto reactionEndEvent = EventListenerCustom::create(endEventName, [=](EventCustom* event)
                {
                    if(reactionName == death)
                    {
                        strikedChar->removeToStage();
                        _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION_SEQUENCE");
                        _eventDispatcher->removeCustomEventListeners("NODE_char" + std::to_string(m_number) + "_END_ACTION_SEQUENCE");
                    }
                    strikedChar->setLocalZOrder(strikedChar->m_index);
                    _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
                });
                auto eventDispatcher = Director::getInstance()->getEventDispatcher();
                eventDispatcher->addEventListenerWithSceneGraphPriority(reactionEndEvent, strikedChar);
                
                
            }
            else
            {
                _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
            }
        }
    });
    
    auto seq = Sequence::create(delayStrike, strikeFunc, NULL);
    this->runAction(seq);
    
    return true;
}
//------------------ACTION HEAL CHAR----------------
bool Character::healChar(std::vector<std::vector<int>> healList)
{
    this->setLocalZOrder(32);
    m_characterDisplay->setAnimation("spell", 1);
    
    auto delayHeal = DelayTime::create(1);
    auto healFunc = CallFunc::create([this, healList]()
    {
        
        this->setLocalZOrder(m_index);
        for(int c = 0; c < healList.size(); c++)
        {
            const auto healedChar = MainObject::getCharByNumber(healList[c][0]);
            
            if(healedChar)
            {
                healedChar->setLocalZOrder(32);
                std::string endEventName = healedChar->setReaction(heal);
                auto reactionEndEvent = EventListenerCustom::create(endEventName, [=](EventCustom* event)
                {
                    printf("heal_end");
                    healedChar->setLocalZOrder(healedChar->m_index);
                    _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
                });
                auto eventDispatcher = Director::getInstance()->getEventDispatcher();
                eventDispatcher->addEventListenerWithSceneGraphPriority(reactionEndEvent, healedChar);
            }
            else
            {
                _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
            }
        }
    });
    auto seq = Sequence::create(delayHeal, healFunc, NULL);
    this->runAction(seq);
    
    return false;
}


std::string Character::setReaction(m_reaction reaction)
{
    std::string animationName = "";
    int nbrLoop = 1;
    bool playLastAnimation = true;
    
    switch (reaction) {
        case block:
            animationName = "block";
            break;
        case pain:
            MainStuff::setCharSpec(m_number, "health", -1);
            animationName = "pain";
            break;
        case death:
            animationName = "death";
            playLastAnimation = false;
            break;
        case heal:
            MainStuff::setCharSpec(m_number, "health", 1);
            animationName = "happy";
            break;
            
        default:
            break;
    }
    
    m_characterDisplay->setAnimation(animationName, nbrLoop, playLastAnimation);
    
    return "CHAR_" + std::to_string(m_number) + "_ANIM_" + animationName + "_END";
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

