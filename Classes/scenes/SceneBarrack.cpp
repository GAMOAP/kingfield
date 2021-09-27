//
//  SceneBarrack.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 01/10/2019.
//

#include "SceneBarrack.hpp"

#include "MainObject.hpp"
#include "MainStuff.hpp"
#include "MainSounds.hpp"

#include "GameDirector.hpp"
#include "GameInfoLayer.hpp"

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
    
    auto gameCharacters = GameCharacters::getInstance();
    gameCharacters->unselectAll(true);
    gameCharacters->setCharacters(5);
    
    GameBoxes::setBoxes();
    
    auto gameCards = GameCards::getInstance();
    gameCards->addDeck();
    gameCards->addSheet();
    gameCards->addLibrary();
    gameCards->unselectAll();
     
    addFightButton();
    addLibraryButton();
}

void SceneBarrack::removeToStage()
{
    GameInfoLayer::addInfoLogo("fight", 1, 2);
    
    removeLibraryButton();
    removeFightButton();
    
    GameDirector::setScene("fight");
}

bool SceneBarrack::allNodeIsIn()
{
    if(!GameCharacters::getCharIsSelected())
    {
        GameCharacters::setCharSelect();
    }
        
    if(!GameCards::getCardSelect())
    {
        GameCards::setCardSelect(8, "sheet");
    }
    else
    {
        GameCards::CardsReseted();
    }
    
    return true;
}

bool SceneBarrack::touchBox(int tag)
{
    if(getIsUsedBox(tag) || (tag == 41) || (tag == 31) || (tag == 35))
    {
        setTouchObject(tag);
        m_touchedBox = tag;
    }
    else if(tag == 32 || tag == 34)
    {
        pushLibraryButton(tag);
        m_touchedBox = tag;
        
        MainSounds::playGame("button_library");
    }
    else if(tag == m_fightButtonTag)
    {
        m_touchedBox = tag;
        m_buttonFight->setTexture(KFSprite::getFile("screen_start_fight_button_down"));
        
        MainSounds::playGame("button_fight");
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
            MainSounds::playGame("fight");
            removeToStage();
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
    m_buttonFight->setTexture(KFSprite::getFile("screen_start_fight_button_up"));
    
    cancelPushLibraryButton();
    
    cancelTouchObject();
    m_touchedBox = NAN;
    
    return true;
}

bool SceneBarrack::cancelTouchBox(int tag)
{
    m_buttonFight->setTexture(KFSprite::getFile("screen_start_fight_button_up"));
    cancelTouchObject();
    return true;
}

bool SceneBarrack::longTouchBox(int tag)
{
    if(getIsUsedBox(tag))
    {
        cancelTouchObject();
        GameDirector::setScene("option", tag);
    }
    return true;
}
//Fight button
void SceneBarrack::addFightButton()
{
    auto box33 = MainObject::getBoxByTag(33);
    
    box33->remove();
    
    auto box33isOutEvent = EventListenerCustom::create("NODE_box33_IS_OUT", [=](EventCustom* event)
    {
        if(!m_buttonFight)
        {
            m_buttonFight = Sprite::create(KFSprite::getFile("screen_start_fight_button_up"));
            m_buttonFight->setPositionY(24);
            m_buttonFight->setTag(33);
            box33->addChild(m_buttonFight);
        }
    });
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(box33isOutEvent, box33);
}
void SceneBarrack::removeFightButton()
{
    auto box33isOutEvent = EventListenerCustom::create("NODE_box33_IS_OUT", [=](EventCustom* event)
    {
        if(m_buttonFight)
        {
            m_buttonFight->removeFromParent();
            m_buttonFight = nullptr;
        }
    });
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(box33isOutEvent, MainObject::getBoxByTag(33));
}

//Library button .............
void SceneBarrack::addLibraryButton()
{
    auto box32 = MainObject::getBoxByTag(32);
    auto box34 = MainObject::getBoxByTag(34);
    
    box32->remove();
    box34->remove();
    
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
