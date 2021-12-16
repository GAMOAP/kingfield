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
#include "MainSounds.hpp"

#include "GameDirector.hpp"

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
        character->initPosition(number);
        return character;
    }
}

bool Character::init(int number)
{
    //Create identity.
    setNumber(number);
    m_className = "char";
    setName(m_className + "_" + std::to_string(number));
    
    m_isActiveThisTurn = false;
    
    if(number >= 5)
    {
        this->setFlip();
        m_isPlayerTeam = false;
    }
    else
    {
        m_isPlayerTeam = true;
    }
    
    initPosition(number);
    
    return true;
}

bool Character::initPosition(int number)
{
    int originTag = m_originTagList[number];
    
    Vec2 positionLC = MainGrid::getLineCollumnByTag(originTag);
    m_line = positionLC.x;
    m_collumn = positionLC.y;
    
    //remove origin box
    if(_tag == originTag)
    {
        return true;
    }
    
    if(_tag == -1)
    {
        addToStage();
        setTag(originTag);
    }
    
    //remove character
    auto IsBoxOutEvent = EventListenerCustom::create("NODE_box" + std::to_string(_tag) + "_IS_IN", [=](EventCustom* event)
    {
        if(!m_characterDisplay)
        {
            remove(1, 1, "both");
        } 
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(IsBoxOutEvent, this);
    
    auto IsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag) + "_IS_OUT", [this,originTag](EventCustom* event)
    {
        if(!m_characterDisplay)
        {
            m_characterDisplay = CharacterDisplay::create(m_number);
            this->addChild(m_characterDisplay, 1);
            setUnselect();
        }
        else
        {
            setNodePosition();
            setVisible(false);
            auto originBox = MainObject::getBoxByTag(originTag);
            originBox->remove();
            auto IsOriginOutEvent = EventListenerCustom::create("NODE_box" +  std::to_string(originTag) + "_IS_OUT", [this, originTag](EventCustom* event)
            {
                MainStuff::initCharSpec(m_number);
                MainStuff::initCardBuff(m_number);
                setVisible(true);
                setTag(originTag);
            });
            _eventDispatcher->addEventListenerWithSceneGraphPriority(IsOriginOutEvent, originBox);
        }
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(IsOutEvent, this);
    
    return true;
}

bool Character::removeToStage()
{
    remove();
    auto boxIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
    {
        m_characterDisplay->stopAnimation();
        if(m_SharedCharacterUI)
        {
            m_SharedCharacterUI = nullptr;
        }
        this->removeFromParent();
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(boxIsOutEvent, this);
    return true;
}

void Character::setStuff(std::string type)
{
    m_characterDisplay->setStuff(type);
}

//---------------------------ACTION--------------------------
bool Character::setAction(KFAction* action)
{
    //init action.
    int actionType = action->getType();
    int actionCard = action->getCardNbr();
    
    printf("[CHAR]actionCard = %i\n",actionCard);
    
    //move character...
    setIsMove(true);
    
    if(actionType == 0)
    {
        int startTag = action->getSartTag();
        int endTag = action->getEndTag();
        setMove(startTag, endTag);
    }
    //character strike...
    if(actionType == 1 || actionType == 2)
    {
        const std::string actionSlotType = action->getSlotSpec();
        std::vector<std::vector<int>> charList = action->getCharStrikedList();
        const int force = action->getCharAttackForce();
        if(force <=0)
        {
            m_characterDisplay->setAnimation("fail", 1);
            auto delayFail = DelayTime::create(1);
            auto failFunc = CallFunc::create([this]()
            {
                m_characterDisplay->setAnimation("stand");
                _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
            });
            auto seq = Sequence::create(delayFail, failFunc, NULL);
            this->runAction(seq);
            return false;
        }
        else
        {
            switch (actionType) {
                case 1:
                    setStrike(charList, actionCard, actionSlotType, force);
                    break;
                case 2:
                    setSpell(charList, actionCard, actionSlotType, force);
                    break;
                    
                    
                default:
                    break;
            }
        }
    }
    
    return true;
}



//------------------ACTION MOVE----------------
bool Character::setMove(int startTag, int endTag)
{
    auto mainGrid = MainGrid::getInstance();
    
    const Vec2 positionLC = mainGrid->getLineCollumnByTag(endTag);
    const int line = positionLC.x;
    const int collumn = positionLC.y;
    
    const Vec3 positionXYZ = mainGrid->getPositionXYZ(line, collumn);
    
    float decTime = 0;
    if(line != m_line && collumn != m_collumn){decTime = m_moveDecTime;}
    if(line < m_line){setLocalZOrder(positionXYZ.z + getIndexClassName() - 1);}
    
    auto move = MoveTo::create(m_moveTime + decTime, Vec2(positionXYZ.x, positionXYZ.y));
    auto delayEndMove = DelayTime::create(m_moveDecTime);
    
    m_line = line;
    m_collumn = collumn;
    
    auto stopFunc = CallFunc::create([this]()
    {
        m_characterDisplay->setAnimation("stand");
    });
    
    auto endFunc = CallFunc::create([this]()
    {
        setNodePosition();
        _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
    });
    
    
    
    auto seq = Sequence::create(move, stopFunc, delayEndMove, endFunc, NULL);
    this->runAction(seq);
    
    m_characterDisplay->setAnimation("walk");
    
    MainSounds::playCharWalk(m_moveTime + decTime, startTag, endTag);
    
    return true;
}
//------------------ACTION STRIKE----------------
bool Character::setStrike(std::vector<std::vector<int>> strikedList, int actionCard, std::string actionSlotType, int force)
{
    auto mainStuff = MainStuff::getInstance();
    
    this->setLocalZOrder(32);
    
    
    if(actionCard == 1)
    {
        m_characterDisplay->setAnimation("spell_" + mainStuff->getStuffByName(m_number, 1)[1], 1);
    }
    else if(actionCard == 2)
    {
        m_characterDisplay->setAnimation("attack_" + mainStuff->getStuffByName(m_number, 2)[1], 1);
        setInfo("attack", force);
    }
    else if(actionCard == 3)
    {
        m_characterDisplay->setAnimation("object_" + mainStuff->getStuffByName(m_number, 3)[1], 1);
    }
    
    auto delayStrike = DelayTime::create(1);
    auto strikeFunc = CallFunc::create([this, strikedList, actionSlotType, force]()
    {
        this->setLocalZOrder(m_index);
        
        for(int c = 0; c < strikedList.size(); c++)
        {
            const auto strikedChar = MainObject::getCharByNumber(strikedList[c][0]);
            m_reaction reactionName;
            if(!strikedChar)
            {
                printf("ERROR :: strike char missing");
            }
            
            strikedChar->setLocalZOrder(32);
            const int defense = strikedList[c][1];
            strikedChar->setInfo("defense", defense);
            
            if(actionSlotType == "strike")
            {
                const int health = MainStuff::getCharSpec(strikedList[c][0])["health"];
                reactionName = block;
                if(force >= defense && !strikedChar->isBlocking())
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
            }
            else if(actionSlotType == "heal")
            {
                if(defense <= 0)
                    reactionName = fail;
                else
                    reactionName = heal;
            }
            else if(actionSlotType == "crystal_break")
            {
                reactionName = block;
                if(force >= defense)
                {
                    reactionName = crystal_break;
                }
            }
            else
            {
                printf("ERROR :: actionSlotType missing");
            }
            
            std::string endEventName = strikedChar->setReaction(reactionName, m_number);
            auto reactionEndEvent = EventListenerCustom::create(endEventName, [=](EventCustom* event)
            {
                setEndAction(strikedChar, reactionName, (int)strikedList.size(), c);
            });
            auto eventDispatcher = Director::getInstance()->getEventDispatcher();
            eventDispatcher->addEventListenerWithSceneGraphPriority(reactionEndEvent, strikedChar);
        }
    });
    
    auto seq = Sequence::create(delayStrike, strikeFunc, NULL);
    this->runAction(seq);
    
    return true;
}

//------------------ACTION SPELL----------------
bool Character::setSpell(std::vector<std::vector<int>> bewitchedList, int actionCard, std::string actionSlotType, int force)
{
    this->setLocalZOrder(32);
    
    auto mainStuff = MainStuff::getInstance();
    
    if(actionCard == 1)
    {
        m_characterDisplay->setAnimation("spell_" + mainStuff->getStuffByName(m_number, 1)[1], 1);
    }
    else if(actionCard == 3)
    {
        m_characterDisplay->setAnimation("object_" + mainStuff->getStuffByName(m_number, 3)[1], 1);
    }
    
    auto delayStrike = DelayTime::create(1);
    auto strikeFunc = CallFunc::create([this, bewitchedList, actionSlotType, force]()
    {
        this->setLocalZOrder(m_index);
        for(int c = 0; c < bewitchedList.size(); c++)
        {
            const auto bewitchedChar = MainObject::getCharByNumber(bewitchedList[c][0]);
            m_reaction reactionName;
            reactionName = block;
            if(actionSlotType == "defense_more"){reactionName = defense_more;}
            if(actionSlotType == "attack_more"){reactionName = attack_more;}
            if(actionSlotType == "defense_less"){reactionName = defense_less;}
            if(actionSlotType == "attack_less"){reactionName = attack_less;}
            if(actionSlotType == "poison"){reactionName = poison;}
            if(actionSlotType == "sleep"){reactionName = sleep;}
            if(actionSlotType == "block"){reactionName = blocking;}
            
            bewitchedChar->setInfo(actionSlotType);
            
            std::string endEventName = bewitchedChar->setReaction(reactionName, m_number);
            auto reactionEndEvent = EventListenerCustom::create(endEventName, [=](EventCustom* event)
            {
                bewitchedChar->setLocalZOrder(bewitchedChar->m_index);
                _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
            });
            auto eventDispatcher = Director::getInstance()->getEventDispatcher();
            eventDispatcher->addEventListenerWithSceneGraphPriority(reactionEndEvent, bewitchedChar);
        }
    });
    
    auto seq = Sequence::create(delayStrike, strikeFunc, NULL);
    this->runAction(seq);
    
    return true;
}
//-----------------END ACTION---------------
bool Character::setEndAction(Character* reactChar, m_reaction reaction, int reactedSize, int reactedNbr)
{
    bool isLastAction = false;
    if(reactedNbr + 1 >= reactedSize)
    {
        isLastAction = true;
    }
    
    std::printf("[ACT]setEndAction, reactedNbr_%i >= reactedSize_%i\n", reactedNbr, reactedSize);
    
    if(reaction == death)
    {
        reactChar->removeToStage();
        int strikedTag = reactChar->getTag();
        auto strikeddBox = MainObject::getBoxByTag(strikedTag);
        auto isIn = EventListenerCustom::create("NODE_box" + std::to_string(strikedTag) + "_IS_IN", [=](EventCustom* event)
        {
            if(isLastAction)
            {
                _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION_SEQUENCE");
                _eventDispatcher->removeCustomEventListeners("NODE_char" + std::to_string(m_number) + "_END_ACTION_SEQUENCE");
            }
        });
        auto eventDispatcher = Director::getInstance()->getEventDispatcher();
        eventDispatcher->addEventListenerWithSceneGraphPriority(isIn,strikeddBox);
    }
    else
    {
        if(isLastAction)
        {
            _eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(m_number) + "_END_ACTION");
        }
    }
    reactChar->setLocalZOrder(reactChar->m_index);
    
    return true;
}

//------------------REACTION----------------
std::string Character::setReaction(m_reaction reaction, int actionCharNbr)
{
    std::string animationName = "stand";
    int nbrLoop = 1;
    bool playLastAnimation = true;
    
    switch (reaction) {
            
        //strike
        case fail:
            animationName = "fail";
            break;
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
    
        //Spell
        case crystal_break:
            MainStuff::setCharSpec(m_number, "crystal", -1);
            animationName = "sad";
            break;
        case defense_more:
            setBuff("defense_more");
            animationName = "happy";
            break;
        case attack_more:
            setBuff("attack_more");
            animationName = "happy";
            break;
        case defense_less:
            setBuff("defense_less");
            animationName = "sad";
            break;
        case attack_less:
            setBuff("attack_less");
            animationName = "sad";
            break;
        case poison:
            setBuff("poison");
            animationName = "sad";
            break;
        case sleep:
            setBuff("sleep");
            animationName = "sad";
            break;
        case blocking:
            setBuff("block");
            animationName = "happy";
            break;
            
        default:
            break;
    }
    m_characterDisplay->setAnimation(animationName, nbrLoop, playLastAnimation, actionCharNbr);
    
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
//------------------BUFF----------------
void Character::setBuff(std::string buffName)
{
    int turnNbr = GameDirector::getScene()->getTurnNumber();
    
    m_buffList.name = buffName;
    if(m_buffList.startTurn == turnNbr)
    {
        m_buffList.endTurn += 2;
    }
    else
    {
        m_buffList.endTurn = turnNbr + 2;
    }
    m_buffList.startTurn = turnNbr;
    
    MainSounds::playChar("buff_" + buffName, m_number);
    
    //printf("turnNbr = %i, m_buffList{name = %s, startTurn = %i, endTurn = %i}\n",turnNbr ,m_buffList.name.c_str(), m_buffList.startTurn, m_buffList.endTurn);
    
    applyBuff(buffName);
}
void Character::manageBuffs()
{
    auto gameDirector = GameDirector::getInstance();
    
    auto mainStuff = MainStuff::getInstance();
    
    int turnNbr = gameDirector->getScene()->getTurnNumber();
    if(m_buffList.endTurn < turnNbr)
    {
        m_buffList.name = "";
        m_buffList.startTurn = 0;
        m_buffList.endTurn = 0;
        applyBuff("NULL");
    }
    
    mainStuff->setLineBuff(m_number, m_line);
    
    
}
bool Character::applyBuff(std::string buffName)
{
    auto mainStuff = MainStuff::getInstance();
    
    if(buffName == "NULL");
    {
        mainStuff->initCombatSpecs(m_number);
        mainStuff->initCardBuff(m_number);
        manageTired();
    }
    if(buffName == "crystal_break")
    {
        
    }
    if(buffName == "attack_more")
    {
        mainStuff->setCharSpec(m_number, "force", 4);
    }
    if(buffName == "defense_more")
    {
        mainStuff->setCharSpec(m_number, "shield", 4);
    }
    if(buffName == "attack_less")
    {
        mainStuff->setCharSpec(m_number, "force", -4);
    }
    if(buffName == "defense_less")
    {
        mainStuff->setCharSpec(m_number, "shield", -4);
    }
    if(buffName == "sleep")
    {
        m_characterDisplay->setState("sleep");
    }
    if(buffName == "poison")
    {
        m_characterDisplay->setState("poison");
        mainStuff->setCardBuff(m_number, "crystal_move", 1);
        mainStuff->setCardBuff(m_number, "crystal_weapon", 1);
    }
    if(buffName == "block")
    {
        m_characterDisplay->setState("block");
    }
    return true;
}

bool Character::manageTired()
{
    auto mainStuff = MainStuff::getInstance();
    auto charSpecs = mainStuff->getCharSpec(m_number);
    
    if(charSpecs["crystal"] <= charSpecs["crystal_red"] && m_characterDisplay->getStateName() == "ok")
    {
        
        m_characterDisplay->setState("tired");
        m_characterDisplay->setAnimation("stand");
    }
    if(charSpecs["crystal"] > charSpecs["crystal_red"] && m_characterDisplay->getStateName() == "tired")
    {
        m_characterDisplay->setState("ok");
        m_characterDisplay->setAnimation("stand");
    }
    
    if(m_characterDisplay->getStateName() == "tired")
    {
        mainStuff->setCharSpec(m_number, "shield", -2);
    }
    
    
    return true;
}

bool Character::isSleeping()
{
    if(m_characterDisplay->getStateName() == "sleep")
        return true;
    else
        return false;
}
bool Character::isBlocking()
{
    if(m_characterDisplay->getStateName() == "block")
        return true;
    else
        return false;
}
std::string Character::getBuffName()
{
    return m_buffList.name;
}
int Character::getBuffTurnLeft()
{
    int gameTurn = GameDirector::getScene()->getTurnNumber();
    
    int turnLeft = (m_buffList.endTurn - gameTurn)/2 + 1;
    
    return turnLeft;
}

void Character::setIfActiveThisTurn(bool isActive)
{
    m_isActiveThisTurn = isActive;
}
bool Character::getIfActiveThisTurn()
{
    return m_isActiveThisTurn;
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

