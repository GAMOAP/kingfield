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
    m_scaleArray.insert(m_scaleArray.end(), {0.9, 1.1, 1.2});
    m_colorSelect = {255, 255, 255};
    m_colorUnselect = {230, 220, 230};
    
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
        isEnemyTeam = false;
    
    if(m_type != type || m_breed != breed || m_object != object || m_isEnemyTeam != isEnemyTeam)
    {
        m_type = type;
        m_breed = breed;
        m_object = object;
        m_isEnemyTeam = isEnemyTeam;
        
        removeCard();
        auto charIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
        {
            setTexture();
            m_cardDisplay->setScale(m_scaleArray[0]);
            m_cardDisplay->setColor(m_colorUnselect);
            m_cardDisplay->flipChessBoard(m_isEnemyTeam);
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(charIsOutEvent, this);
    }
    
    return true;
}
