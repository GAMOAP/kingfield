//
//  SheetCard.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 17/07/2019.
//
#include "Constants.h"

#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainStuff.hpp"

#include "GameCharacters.hpp"

#include "SheetCard.hpp"

USING_NS_CC;

SheetCard* SheetCard::setCard(int number, std::string board)
{
    auto card = dynamic_cast<SheetCard*>(MainObject::getCardByNumber(number, board));
    if(!card)
    {
        SheetCard* ret = new (std::nothrow) SheetCard();
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

bool SheetCard::init(int number)
{
    m_tagList.insert(m_tagList.end(), {41, 51, 61, 45, 55});
    m_scaleArray.insert(m_scaleArray.end(), {0.8, 1, 1.2});
    m_colorSelect = {255, 255, 255};
    m_colorUnselect = {220, 210, 210};
    
    setNumber(number);
    m_board = "sheet";
    int dec_m_number = 4;
    setTag(m_tagList[m_number - dec_m_number]);
    
    initPosition();
    
    return true;
}

bool SheetCard::initDisplay()
{
    int charSelectNumber = GameCharacters::getCharSelect()->getNumber();
    
    std::vector<std::string> cardDetails = MainStuff::getStuffByName(charSelectNumber, m_number);
    
    std::string type = CARD_TYPE[m_number];
    std::string breed = cardDetails[0];
    std::string object = cardDetails[1];
    
    if(m_type != type || m_breed != breed || m_object != object)
    {
        m_type = type;
        m_breed = breed;
        m_object = object;
        
        removeCard();
        auto charIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
        {
            setTexture();
            m_cardDisplay->setScale(m_scaleArray[0]);
            m_cardDisplay->setColor(m_colorUnselect);
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(charIsOutEvent, this);
    }
    
    return true;
}
