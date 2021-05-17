//
//  GameCharacters.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//
#include "MainObject.hpp"
#include "MainStuff.hpp"
#include "MainAction.hpp"

#include "GameDirector.hpp"

#include "GameBoxes.hpp"
#include "GameCards.hpp"

#include "GameCharacters.hpp"

#include <iostream>

USING_NS_CC;

GameCharacters* GameCharacters::getInstance()
{
    if (!m_SharedGameCharacters)
    {
        m_SharedGameCharacters = new (std::nothrow) GameCharacters;
        m_SharedGameCharacters->init();
    }
    return m_SharedGameCharacters;
}

bool GameCharacters::init()
{
    return true;
}

void GameCharacters::setCharacters(int charNumbers)
{
    if(!m_SharedGameCharacters){ getInstance();}
    
    for(int c = 0; c < CHAR_NUMBER ; c++)
    {
        auto character = MainObject::getCharByNumber(c);
        if(character && c >= charNumbers)
        {
            character->removeToStage();
            character = nullptr;
        }
        
        if(c < charNumbers)
        {
            m_SharedGameCharacters->initChar(c);
        }
    }
}

bool GameCharacters::initChar(int charNbr)
{
    auto character = Character::setCharacter(charNbr);
    if(charNbr == 2){m_SharedGameCharacters->m_kingFriend = character;}
    if(charNbr == 7){m_SharedGameCharacters->m_kingEnemy = character;}
    
    MainStuff::initCharSpec(charNbr);
    MainStuff::initCardBuff(charNbr);
    
    m_SharedGameCharacters->m_charactersTagsMemory[charNbr] = m_SharedGameCharacters->m_charactersTagsMemoryOrigin[charNbr];
    
    return true;
}

void GameCharacters::removeAllCharacters()
{
    auto nodeList = MainObject::getMainLayer()->getChildren();
    Vector<Node*>::iterator nlIt;
    for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
    {
        auto character = dynamic_cast<Character*>(*nlIt);
        if(character)
        {
            GameBoxes::setBoxUnselect(character->getTag());
            character->removeToStage();
            character = nullptr;
        }
    }
    m_SharedGameCharacters->m_charSelected = nullptr;
    if(m_SharedGameCharacters->m_characterUI)
    {
        m_SharedGameCharacters->m_characterUI->removeCharacterUI();
    }
}
void GameCharacters::removeCharacter(int charNbr)
{
    auto character = MainObject::getCharByNumber(charNbr);
    if(character)
    {
        GameBoxes::setBoxUnselect(character->getTag());
        character->removeToStage();
        character = nullptr;
    }
}

//----------------------------CHARACTER SELECT------------------------------------
void GameCharacters::setCharSelect(int number)
{
    if(number < 0)
    {
        number = 2;
        if(m_SharedGameCharacters->m_charNumberMemory >= 0)
        {
            number = m_SharedGameCharacters->m_charNumberMemory;
        }
    }
    
    auto charSelected = m_SharedGameCharacters->m_charSelected;
    if(charSelected)
        m_SharedGameCharacters->m_charNumberMemory = charSelected->getNumber();
    
    if(!charSelected || charSelected->getNumber() != number)
    {
        auto sceneChildren = MainObject::getMainLayer()->getChildren();
        Vector<Node*>::iterator scIt;
        for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
        {
            auto character = dynamic_cast<Character*>(*scIt);
            if(character)
            {
                if(character->getNumber() == number)
                {
                    m_SharedGameCharacters->m_charSelected = character;
                    bool isPlayerTurn = GameDirector::getScene()->getIsPlayerTurn();
                    if(isPlayerTurn)
                    {
                        GameBoxes::setBoxSelect(character->getTag());
                    }
                    
                    character->setSelect(isPlayerTurn);
                    m_SharedGameCharacters->m_characterUI = CharacterUI::setCharacterUI(number);
                    GameCards::resetCards();
                }
                else
                {
                    GameBoxes::setBoxUnselect(character->getTag());
                    character->setUnselect();
                }
            }
        }
    }
}
Character* GameCharacters::getCharSelect()
{
    Character* character = m_SharedGameCharacters->m_charSelected;
    if(!m_SharedGameCharacters->m_charSelected)
    {
        int charSelectNbrMem = m_SharedGameCharacters->m_charNumberMemory;
        if(charSelectNbrMem < 0)
            charSelectNbrMem = 2;
        
        character = MainObject::getCharByNumber(charSelectNbrMem);
    }
    return character;
}
bool GameCharacters::getCharIsSelected()
{
    if(!m_SharedGameCharacters->m_charSelected)
        return false;
    else
        return true;
}
void GameCharacters::unselectAll(bool eraseMemory)
{
    auto charSelected = m_SharedGameCharacters->m_charSelected;
    
    if(charSelected)
    {
        m_SharedGameCharacters->m_charNumberMemory = charSelected->getNumber();
        
        GameBoxes::setBoxUnselect(m_SharedGameCharacters->m_charSelected->getTag());
    }
    
    if(eraseMemory)
    {
        m_SharedGameCharacters->m_charNumberMemory = -1;
    }
    
    if(m_SharedGameCharacters->m_charSelected)
    {
        m_SharedGameCharacters->m_charSelected->setUnselect();
        m_SharedGameCharacters->m_charSelected = nullptr;
    }
    
    if(m_SharedGameCharacters->m_characterUI)
    {
        m_SharedGameCharacters->m_characterUI->removeCharacterUI();
        m_SharedGameCharacters->m_characterUI = nullptr;
    }
}
//----------------------------KING-------------------------------
Character* GameCharacters::getKingFriend()
{
    return m_SharedGameCharacters->m_kingFriend;
}

//-------------------------CHAR MEMORY----------------------------
int GameCharacters::getCharNumberMemory()
{
    return m_SharedGameCharacters->m_charNumberMemory;
}

int GameCharacters::getCharTagMemory(int charNbr)
{
    return m_SharedGameCharacters->m_charactersTagsMemory[charNbr];
}

//---------------------------ACTION-------------------------------
void GameCharacters::setAction(std::vector<KFAction*> actionSequence)
{
    m_SharedGameCharacters->m_isActionRun = true;
    
    //unselet all, move box in place and set action when nodes are in place.
    
    m_SharedGameCharacters->unselectAll();
    GameCards::unselectAll();
    GameBoxes::unselectAll();
    
    auto character = MainObject::getCharByNumber(actionSequence[0]->getCharNbr());
    
    auto charIsPlace = EventListenerCustom::create("NODE_char" + std::to_string(character->getTag())+"_IS_PLACE", [=](EventCustom* event)
    {
        m_SharedGameCharacters->m_sequenceState = 0;
        m_SharedGameCharacters->m_actionSequence = actionSequence;
        m_SharedGameCharacters->setActionSequence(character);
    });
    
    auto endActionSequence = EventListenerCustom::create("NODE_char" + std::to_string(character->getNumber()) + "_END_ACTION_SEQUENCE", [=](EventCustom* event)
    {
        m_SharedGameCharacters->endActionSequence(character);
    });
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(endActionSequence, character);
    eventDispatcher->addEventListenerWithSceneGraphPriority(charIsPlace, character);
    
    if(character && character->getIsPlace())
    {
        eventDispatcher->dispatchCustomEvent("NODE_char" + std::to_string(character->getTag())+"_IS_PLACE");
        eventDispatcher->removeCustomEventListeners("NODE_char" + std::to_string(character->getTag())+"_IS_PLACE");
    }
}
void GameCharacters::setActionSequence(Character* character)
{
    if(m_sequenceState >= m_actionSequence.size())
    {
        character->getEventDispatcher()->dispatchCustomEvent("NODE_char" + std::to_string(character->getNumber()) + "_END_ACTION_SEQUENCE");
        character->getEventDispatcher()->removeCustomEventListeners("NODE_char" + std::to_string(character->getNumber()) + "_END_ACTION_SEQUENCE");
    }
    else
    {
        auto action = m_actionSequence[m_sequenceState];
        character->setIsMove(true);
        character->setAction(action);
        
        auto charIsActionEnd = EventListenerCustom::create("NODE_char" + std::to_string(character->getNumber()) + "_END_ACTION", [=](EventCustom* event)
        {
            m_SharedGameCharacters->m_charactersTagsMemory[character->getNumber()] = character->getTag();
            
            character->getEventDispatcher()->removeCustomEventListeners("NODE_char" + std::to_string(character->getNumber()) + "_END_ACTION");
            action->~KFAction();
            
            m_sequenceState++;
            setActionSequence(character);
        });
        auto eventDispatcher = Director::getInstance()->getEventDispatcher();
        eventDispatcher->addEventListenerWithSceneGraphPriority(charIsActionEnd, character);
    }
}
void GameCharacters::endActionSequence(Character* character)
{
    
    character->getEventDispatcher()->removeCustomEventListeners("NODE_char" + std::to_string(character->getNumber()) + "_END_ACTION_SEQUENCE");
    
    for(int c = 0; c < CHAR_NUMBER; c++)
    {
        auto deadChar = MainObject::getCharByNumber(c);
        if(c == 2 && !deadChar)
        {
            GameDirector::stopFight(false);
        }
        else if(c == 7 && !deadChar)
        {
            GameDirector::stopFight(true);
        }
    }
    
    MainStuff::setCharSpec(character->getNumber(), "crystal", -m_actionSequence[0]->getCost());
    m_actionSequence.clear();
    character->setIsMove(false);
    
    m_SharedGameCharacters->m_isActionRun = false;
    
    GameDirector::endTurn();
}

void GameCharacters::setActionAll(std::string actionName)
{
    auto sceneChildren = MainObject::getMainLayer()->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto character = dynamic_cast<Character*>(*scIt);
        if(character)
        {
            if(actionName == "give_crystals" &&
               GameDirector::getScene()->getIsTeamTurn(character->getNumber()))
            {
                MainStuff::setCharSpec(character->getNumber(), "crystal", +1);
            }
            if(actionName == "manage_buffs")
            {
                character->manageBuffs();
            }
        }
    }
}
bool GameCharacters::getIsActionRun()
{
    return m_SharedGameCharacters->m_isActionRun;
}
//---------------------------------CHARACTER UI------------------------------
CharacterUI* GameCharacters::getCharUI()
{
    return m_SharedGameCharacters->m_characterUI;
}
