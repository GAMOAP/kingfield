//
//  Card.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 12/07/2019.
//

#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainStuff.hpp"

#include "GameDirector.hpp"

#include "GameBoxes.hpp"
#include "GameCharacters.hpp"

#include "Card.hpp"

#include <iostream>

USING_NS_CC;

Card* Card::getClassObject()
{
    return this;
}

bool Card::initPosition()
{
    m_className = "card";
    
    setName(m_className + "_" + m_board + "_" + std::to_string(m_number));
    
    Vec2 positionLC = MainGrid::getLineCollumnByTag(_tag);
    m_line = positionLC.x;
    m_collumn = positionLC.y;
    
    addToStage();
    
    initDisplay();
    
    return true;
}

void Card::setTexture(bool popUp)
{
    if(!m_cardDisplay)
    {
        m_cardDisplay = CardDisplay::create(m_type, m_breed, m_object, m_board);
        this->addChild(m_cardDisplay);
    }
    else
    {
        m_cardDisplay->setTexture(m_type, m_breed, m_object, popUp);
    }
}

//------------------------SELECT UNSELECT-----------------------
void Card::setSelect()
{
    if(m_cardDisplay && m_board != "library")
    {
        m_select = true;
        up();
        auto charIsOutUp = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_UP", [this](EventCustom* event)
        {
            m_cardDisplay->setScale(m_scaleArray[1]);
            m_select = true;
        });
        m_cardDisplay->setSelect();
        m_cardDisplay->setColor(m_colorSelect);
        
        GameBoxes::setBoxSelect(getTag());
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(charIsOutUp, this);
    }
}
void Card::setUnselect()
{
    if(m_cardDisplay && m_board != "library")
    {
        m_select = false;
        m_cardDisplay->setUnselect();
        m_cardDisplay->setScale(m_scaleArray[0]);
        m_cardDisplay->setColor(m_colorUnselect);
        
        GameBoxes::setBoxUnselect(getTag());
    }
}
void Card::removeCard()
{
    if(!m_cardDisplay)
        remove();
    else
    {
        float r = random(0, 100);
        float rv = r / 100;
        remove(rv, 0);
    }
}

CardDisplay* Card::getCardDisplay(){ return m_cardDisplay;}
std::string Card::getBoard(){ return m_board;}
std::string Card::getType(){ return m_type;}
std::string Card::getBreed(){ return m_breed;}
std::string Card::getObject(){ return m_object;}

int Card::getCost()
{
    int charNbr = GameCharacters::getCharSelect()->getNumber();
    return MainStuff::getCardSpec(m_type, m_breed, m_object, charNbr)->getMana(false);
}

bool Card::isEqual(const Card* b)const
{
    return (m_type == b->m_type && m_breed == b->m_breed && m_object == b->m_object);
}
