//
//  SceneBarrack.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 01/10/2019.
//

#include "SceneBarrack.hpp"

#include "MainObject.hpp"
#include "MainStuff.hpp"

#include "MainDirector.hpp"
#include "MainInfoLayer.hpp"

#include "GameBoxes.hpp"
#include "GameCharacters.hpp"
#include "GameCards.hpp"

#include <iostream>

USING_NS_CC;

SceneBarrack* SceneBarrack::setScene()
{
    if (!m_SharedSceneBarrack)
    {
        m_SharedSceneBarrack = new (std::nothrow) SceneBarrack;
        m_SharedSceneBarrack->init();
    }
    return m_SharedSceneBarrack;
}

bool SceneBarrack::init()
{
    setName("barrack");
    return true;
}

void SceneBarrack::addToStage()
{
    MainStuff::initCharStuffList();
    
    GameBoxes::setBoxes();
    
    auto gameCharacters = GameCharacters::getInstance();
    gameCharacters->setCharacters(5);
    gameCharacters->unselectAll();
    
    auto gameCards = GameCards::getInstance();
    gameCards->addDeck();
    gameCards->addSheet();
    gameCards->addLibrary();
    gameCards->unselectAll();
    
    addLibraryButton();
}

void SceneBarrack::removeToStage()
{
    removeLibraryButton();
    
    MainDirector::setScene("fight");
}

bool SceneBarrack::allNodeIsIn()
{
    if(!GameCharacters::getCharIsSelected())
        GameCharacters::setCharSelect();
        
    if(!GameCards::getCardSelect())
        GameCards::setCardSelect(8, "sheet");
    else
        GameCards::CardsReseted();
    
    return true;
}

bool SceneBarrack::touchBox(int tag)
{
    if(getIsUsedBox(tag) || (tag >= 22 && tag <= 24))
    {
        setTouchObject(tag);
        m_touchedBox = tag;
    }
    else if(tag == 32 || tag == 34)
    {
        pushLibraryButton(tag);
        m_touchedBox = tag;
    }
    else
        m_touchedBox = NAN;
    
    return true;
}

bool SceneBarrack::unTouchBox(int tag)
{
    if(tag == m_touchedBox)
    {
        //select character.
        auto character = MainObject::getCharByTag(tag);
        if(character)
        {
            GameCharacters::setCharSelect(character->getNumber());
            MainStuff::resetCardSpecManaCost();
        }
        //select card.
        auto card = GameCards::getCardByTag(tag);
        if(card)
        {
            if(card->getBoard() == "library")
                GameCards::setCardSelectLibrary(card);
            else
                GameCards::setCardSelect(card->getNumber(), card->getBoard());
        }
        //fight button
        if(tag == m_fightButtonTag)
        {
            auto box32isOutEvent = EventListenerCustom::create("NODE_box23_IS_PLACE", [=](EventCustom* event)
            {
                removeToStage();
            });
            auto eventDispatcher = Director::getInstance()->getEventDispatcher();
            eventDispatcher->addEventListenerWithSceneGraphPriority(box32isOutEvent, MainObject::getBoxByTag(m_fightButtonTag));
        }
        //library buttons.
        if(tag == m_libraryLeftButtonTag || tag == m_libraryRightButtonTag)
        {
            int pagePlus = 1;
            if(tag == m_libraryLeftButtonTag)
                pagePlus = -1;
            
            GameCards::setLibraryPage(pagePlus);
        }
    }
    cancelPushLibraryButton();
    
    cancelTouchObject();
    m_touchedBox = NAN;
    
    return true;
}

bool SceneBarrack::cancelTouchBox(int tag)
{
    cancelTouchObject();
    return true;
}

bool SceneBarrack::longTouchBox(int tag)
{
    if(getIsUsedBox(tag))
    {
        cancelTouchObject();
        MainDirector::setScene("option", tag);
    }
    return true;
}

//Library button .............
void SceneBarrack::addLibraryButton()
{
    auto box32 = MainObject::getBoxByTag(32);
    auto box34 = MainObject::getBoxByTag(34);
    
    auto box32isOutEvent = EventListenerCustom::create("NODE_box32_IS_OUT", [=](EventCustom* event)
    {
        if(!m_libraryButtonLeft)
        {
            m_libraryButtonLeft = Sprite::create(KFSprite::getFile("screen_library_button_left"));
            m_libraryButtonLeft->setPositionY(16);
            m_libraryButtonLeft->setTag(32);
            box32->addChild(m_libraryButtonLeft);
        }
    });
    
    auto box34isOutEvent = EventListenerCustom::create("NODE_box34_IS_OUT", [=](EventCustom* event)
    {
        if(!m_libraryButtonRight)
        {
            m_libraryButtonRight = Sprite::create(KFSprite::getFile("screen_library_button_right"));
            m_libraryButtonRight->setPositionY(16);
            m_libraryButtonRight->setTag(34);
            box34->addChild(m_libraryButtonRight);
        }
    });
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(box32isOutEvent, box32);
    eventDispatcher->addEventListenerWithSceneGraphPriority(box34isOutEvent, box34);
}
void SceneBarrack::removeLibraryButton()
{
    auto box32isOutEvent = EventListenerCustom::create("NODE_box32_IS_OUT", [=](EventCustom* event)
    {
        if(m_libraryButtonLeft)
        {
            m_libraryButtonLeft->removeFromParent();
            m_libraryButtonLeft = nullptr;
        }
    });
    
    auto box34isOutEvent = EventListenerCustom::create("NODE_box34_IS_OUT", [=](EventCustom* event)
    {
        if(m_libraryButtonRight)
        {
            m_libraryButtonRight->removeFromParent();
            m_libraryButtonRight = nullptr;
        }
    });
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(box32isOutEvent, MainObject::getBoxByTag(32));
    eventDispatcher->addEventListenerWithSceneGraphPriority(box34isOutEvent, MainObject::getBoxByTag(34));
}
void SceneBarrack::pushLibraryButton(int tag)
{
    if(tag == 32)
        m_libraryButtonLeft->setScale(0.9);
    
    if(tag == 34)
        m_libraryButtonRight->setScale(0.9);
}
void SceneBarrack::cancelPushLibraryButton()
{
    m_libraryButtonLeft->setScale(1);
    m_libraryButtonRight->setScale(1);
}
