//
//  DeckCard.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 17/07/2019.
//
#include "Constants.h"

#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainStuff.hpp"

#include "GameCharacters.hpp"

#include "DeckCard.hpp"

#include <iostream>

USING_NS_CC;

DeckCard* DeckCard::setCard(int number, std::string board)
{
    auto card = dynamic_cast<DeckCard*>(MainObject::getCardByNumber(number, board));
    if(!card)
    {
        DeckCard* ret = new (std::nothrow) DeckCard();
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
        return ret;
    }
    else
    {
        card->initDisplay();
        return card;
    }
}

bool DeckCard::init(int number)
{
    m_tagList.insert(m_tagList.end(), {12, 13, 14, 15});
    
    setNumber(number);
    m_board = "deck";
    setTag(m_tagList[m_number]);
    
    initPosition();
    
    return true;
}

bool DeckCard::initDisplay()
{
    int charSelectNumber = GameCharacters::getCharSelect()->getNumber();
    
    std::vector<std::string> cardDetails = MainStuff::getStuffByName(charSelectNumber, m_number);
    
    std::string type = CARD_TYPE[m_number];
    std::string breed = cardDetails[0];
    std::string object = cardDetails[1];
    bool isEnemyTeam = true;
    if(charSelectNumber < 5)
    {
        isEnemyTeam = false;
    }
    if(m_type != type || m_breed != breed || m_object != object || m_isEnemyTeam != isEnemyTeam)
    {
        m_type = type;
        m_breed = breed;
        m_object = object;
        m_isEnemyTeam = isEnemyTeam;
        
        removeCard();
        auto charIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
        {
            setTexture(NORMAL);
            m_cardDisplay->setUnselect(m_board);
            m_cardDisplay->flipChessBoard(m_isEnemyTeam);
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(charIsOutEvent, this);
    }
    else
    {
        setTexture(POPUP);
    }
    
    return true;
}

void Card::initLevelUp()
{
    int charSelectNumber = GameCharacters::getCharSelect()->getNumber();
    
    std::vector<std::string> cardDetails = MainStuff::getStuffByName(charSelectNumber, m_number);
    
    m_type = CARD_TYPE[m_number];
    m_breed = cardDetails[0];
    m_object = cardDetails[1];
    
    removeCard();
    
    auto cardIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
    {
        setTexture(LEVELUP);
        auto cardIsInEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_IN", [this](EventCustom* event)
        {
            setTexture(POPLEVELUP);
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(cardIsInEvent, this);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(cardIsOutEvent, this);
}
