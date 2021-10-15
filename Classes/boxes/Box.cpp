//
//  Box.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 03/06/2019.
//
#include "Constants.h"

#include "MainGrid.hpp"
#include "MainObject.hpp"

#include "GameDirector.hpp"

#include "Box.hpp"

#include <iostream>

USING_NS_CC;

Box* Box::getClassObject()
{
    return this;
}

Box* Box::setBox(int line, int collumn)
{
    auto box = MainObject::getBoxByLC(line, collumn);
    if(!box)
    {
        Box* ret = new (std::nothrow) Box();
        if(ret && ret->init(line, collumn))
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
        box->initDisplay();
        return box;
    }
}

bool Box::init(int line, int collumn)
{
//Create identity.
    m_className = "box";
    
    m_line = line;
    m_collumn = collumn;
    setTag(MainGrid::getTag(Vec2(m_line, m_collumn)));
    setName(m_className + "_" + std::to_string(_tag));
    
    setCascadeColorEnabled(true);
    
    addToStage();
    
    initDisplay();
    
    return true;
}

void Box::initDisplay()
{
    auto gameDirector = GameDirector::getInstance();
    m_scene = gameDirector->getScene()->getName();
    
    std::string type = "background";
    std::string breed = gameDirector->getKingBreed();
    std::string secondBreed = gameDirector->getKingBreed();
    
    if(m_scene == "barrack")
    {
        for(int f = 0; f < m_barrackFieldTag.size(); f++){
            if(_tag == m_barrackFieldTag[f]){
                type = "field";
            }
        }
        for(int b = 0; b < m_barrackBarrackTag.size(); b++){
            if(_tag == m_barrackBarrackTag[b]){
                type = "barrack";
            }
        }
    }
    else if(m_scene == "fight" )
    {
        if(_tag >= 31 && _tag != 36 && _tag != 40)
        {
            secondBreed = gameDirector->getEnemyKingBreed();
        }
        
        for(int f = 0; f < m_fightFieldTag.size(); f++){
            if(_tag == m_fightFieldTag[f]){
                type = "field";
            }
        }
        for(int b = 0; b < m_fightBarrackTag.size(); b++){
            if(_tag == m_fightBarrackTag[b]){
                type = "barrack";
            }
        }
    }
    
    if(type == "background")
    {
        breed = "void";
        secondBreed = "void";
    }
    
    if(type != m_type || breed != m_breed || secondBreed != m_secondBreed || m_isRumble == true)
    {
        m_type = type;
        m_breed = breed;
        m_secondBreed = secondBreed;
        
        removeBox();
    }
}

void Box::removeBox()
{
    float speedFactor = 1;
    float delayFactor = 1;
    std::string movement = "both";
    for(int l = 0; l < m_libraryTag.size(); l++){
        if(_tag == m_libraryTag[l])
        {
            if(m_scene == "barrack")
            {
                speedFactor = 2;//change valor in LibraryCard::initDisplay() & 
                movement = "add";
            }
            if(m_scene == "fight")
            {
                delayFactor = 0;
                movement = "remove";
            }
        }
    }
    
    //remove box from screen
    remove(speedFactor,delayFactor, movement);
    
    auto boxIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
    {
        if(!m_boxDisplay)
        {
            m_boxDisplay = BoxDisplay::create(m_line, m_collumn, m_type, m_breed, m_secondBreed, m_scene);
            m_boxDisplay->setPosition(Vec2(0, m_centerHeigth));
            this->addChild(m_boxDisplay, 0);
            
            m_boxDisplay->setColor(m_colorUnselect);
        }
        else
        {
            m_boxDisplay->setTexture(m_line, m_collumn, m_type, m_breed, m_secondBreed, m_scene);
        }
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(boxIsOutEvent, this);
}

//SELECT UNSELECT-----------------------
void Box::setSelect(float speedFactor)
{
    if(m_boxDisplay)
    {
        m_select = true;
        m_boxDisplay->setColor(m_colorSelect);
        this->up(speedFactor);
    }
}
void Box::setUnselect()
{
    if(m_boxDisplay)
    {
        m_select = false;
        m_boxDisplay->setColor(m_colorUnselect);
        this->place();
    }
}

//RUMBLE------------------------------------
void Box::startRumble()
{
    //initDisplay();
    
    m_isRumble = true;
    rumble();
}
void Box::stopRumble()
{
    initDisplay();
    m_isRumble = false;
}
void Box::rumble()
{
    if(m_isRumble)
    {
        remove(4);
    }
        
    auto boxIsOut = EventListenerCustom::create("NODE_box" + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
    {
        const std::string scene = "rumble";
        const std::string type = m_type;
        
        const unsigned long s = BREED_TYPE.size() - 1;
        const int rBreed = random(0, int(s));
        const int rSecondBreed = random(0, int(s));
        
        const std::string breed = BREED_TYPE[rBreed];
        const std::string secondBreed = BREED_TYPE[rSecondBreed];
        
        //printf("breed = %s:%i, secondBreed = %s:%i\n",breed.c_str(),rBreed, secondBreed.c_str(),rSecondBreed);
        
        m_boxDisplay->setTexture(m_line, m_collumn, type, breed, secondBreed, scene);
        
        
        auto boxIsIn = EventListenerCustom::create("NODE_box" + std::to_string(_tag)+"_IS_IN", [this](EventCustom* event)
        {
            if(m_isRumble)
                rumble();
        });
        _eventDispatcher->addEventListenerWithSceneGraphPriority(boxIsIn, this);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(boxIsOut, this);
}
bool Box::getIsRumble()
{
    return m_isRumble;
}

//ACTION------------------------------------
void Box::setActionUI(int actionType, bool touchAuth)
{
    std::string authColor = "";
    if(!touchAuth)
    {
        authColor = "_gray";
        m_touchAuth = false;
    }
    else{
        m_touchAuth = true;
    }
    
    std::string actionColor;
    switch (actionType) {
        case 0:
            actionColor = "blue";
            break;
        case 1:
            actionColor = "red";
            break;
        case 2:
            actionColor = "green";
            break;
            
        default:
            break;
    }
    
    float r = random(0, 20);
    float rt = r / 100;
    auto delay = DelayTime::create(rt);
    auto callFunc = CallFunc::create([=]()
    {
        if(touchAuth)
        {
            setSelect(0.5);
        }
        std::string fileName = "UI/char/box_move_" + actionColor + authColor + ".png";
        if(!m_boxActionUI)
        {
            m_boxActionUI = Sprite::createWithSpriteFrameName(fileName);
            m_boxActionUI->setName("board_action");
            this->addChild(m_boxActionUI);
        }
        else
            m_boxActionUI->setTexture(fileName);
    });
    auto seq = Sequence::create(delay, callFunc, NULL);
    this->runAction(seq);
}
void Box::resetActionUI()
{
    setUnselect();
    
    if(m_boxActionUI)
    {
        m_boxActionUI->removeFromParent();
        m_boxActionUI = nullptr;
    }
}

bool Box::getIsActionUI()
{
    if(m_boxActionUI)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::string Box::getBreed()
{
    return m_breed;
}

std::string Box::getType()
{
    return m_type;
}
