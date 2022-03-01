//
//  LibraryCard.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 17/07/2019.
//
#include "Constants.h"

#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainStuff.hpp"
#include "MainSounds.hpp"

#include "GameCards.hpp"

#include "LibraryCard.hpp"

#include <iostream>

USING_NS_CC;

LibraryCard* LibraryCard::setCard(int number, std::string board)
{
    auto card = dynamic_cast<LibraryCard*>(MainObject::getCardByNumber(number, board));
    if(!card)
    {
        LibraryCard* ret = new (std::nothrow) LibraryCard();
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
        card->setPage(0);
        return card;
    }
}

bool LibraryCard::init(int number)
{
    m_tagList.insert(m_tagList.end(), {62, 63, 64, 52, 53, 54, 42, 43, 44});
    
    setNumber(number);
    m_board = "library";
    setTag(m_tagList[m_number]);
    
    float rot = random(-3, 3);
    this->setRotation(rot);
    
    setPage(0);
    
    initPosition();
    
    return true;
}

bool LibraryCard::initDisplay()
{
    const unsigned long nbrBreed = BREED_TYPE.size();
    const int number = m_number + m_page * 9;
    const int lib_C = number / nbrBreed;
    const unsigned long lib_L = number - lib_C * nbrBreed;
    
    const auto cardSelect = GameCards::getCardSelect();
    std::string type = CARD_TYPE[8];
    if(cardSelect){
        type = cardSelect->getType();
    }
    
    std::string breed = BREED_TYPE[lib_L];
    
    std::string object = "";
    if(type != "job" && type != "breed")
        object = BREED_TYPE[lib_C];
    
    bool isVisible = true;
    if(((type == "job" || type == "breed") && lib_C > 0) || (lib_L > nbrBreed && lib_C > nbrBreed))
        isVisible = false;
    
    if(m_type != type || m_breed != breed || m_object != object)
    {
        m_type = type;
        m_breed = breed;
        m_object = object;
        
        if(!m_cardDisplay)
        {
            float speedFactor = 2; //change valor in Box::removeBox()
            float delayFactor = 1;
            std::string movement = "add";
            
            remove(speedFactor, delayFactor, movement);
            auto cardIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [=](EventCustom* event)
            {
                setTexture();
                m_cardDisplay->setUnselect(m_board);
                
                setVisible(isVisible);
            });
            _eventDispatcher->addEventListenerWithSceneGraphPriority(cardIsOutEvent, this);
        }
        else
        {
            place(0,false);
            this->getActionManager()->removeAction(m_rotSeq);
            
            float r = random(20, 40);
            float rt = r / 200;
            float rot = random(-10, 10);
            auto scaleOut = ScaleTo::create(rt, 0.2, 1);
            auto scaleIn = ScaleTo::create(rt, 1, 1);
            auto rotate = RotateTo::create(rt, rot);
            auto spawn = Spawn::createWithTwoActions(scaleOut, rotate);
            auto respawn = Spawn::createWithTwoActions(scaleIn, rotate);
            auto callFunc = CallFunc::create([=]()
            {
                setTexture();
                setVisible(isVisible);
                setUnselect();
            });
            auto callEndFunc= CallFunc::create([=]()
            {
                setLibraryCardSelect();
            });
            auto m_rotSeq = Sequence::create(spawn, callFunc, respawn, callEndFunc, NULL);
            this->runAction(m_rotSeq);
            MainSounds::playCard("flip", m_number);
        }
    }
    
    return true;
}

void LibraryCard::setSelect()
{
    if(m_cardDisplay)
    {
        m_select = true;
        up(1, false);
        m_cardDisplay->setSelect(m_board);
    }
}

void LibraryCard::setUnselect()
{
    if(m_cardDisplay)
    {
        m_select = false;
        m_cardDisplay->setUnselect(m_board);
    }
}

void LibraryCard::setPage(int pageNbr)
{
    m_page = pageNbr;
}
int LibraryCard::getPage()
{
    return m_page;
}

void LibraryCard::setLibraryCardSelect()
{
    const auto cardSelect = GameCards::getCardSelect();
    if(cardSelect && isVisible() && m_breed == cardSelect->getBreed() && m_object == cardSelect->getObject())
    {
        setSelect();
    }
}
