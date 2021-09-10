//
//  GameBoxes.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 04/07/2019.
//

#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainStuff.hpp"
#include "MainAction.hpp"

#include "GameDirector.hpp"
#include "GameCharacters.hpp"
#include "GameCards.hpp"

#include "GameBoxes.hpp"

#include <iostream>

USING_NS_CC;

GameBoxes* GameBoxes::getInstance()
{
    if (!m_SharedGameBoxes)
    {
        m_SharedGameBoxes = new (std::nothrow) GameBoxes;
        m_SharedGameBoxes->init();
    }
    return m_SharedGameBoxes;
}

bool GameBoxes::init()
{
    
    return true;
}

void GameBoxes::setBoxes()
{
    auto mainGrid = MainGrid::getInstance();
    
    for(int l = mainGrid->getOriginLine(); l < mainGrid->getLineNumber(); l++)
    {
        for(int c = mainGrid->getOriginCollumn(); c < mainGrid->getCollumnNumber(); c++)
        {
            auto box = Box::setBox(l, c);
        }
    }
}

//-----------------------------------BOX SELECTION---------------------------
void GameBoxes::setBoxSelect(int tag)
{
    auto box = MainObject::getBoxByTag(tag);
    if(box)
        box->setSelect();
}
void GameBoxes::setBoxUnselect(int tag)
{
    auto box = MainObject::getBoxByTag(tag);
    if(box)
        box->setUnselect();
}

void GameBoxes::setFightButtonBox(bool isAuth)
{
    printf("isAuth = %i", isAuth);
    auto box = MainObject::getBoxByTag(65);
    if(!isAuth)
    {
        box->down();
        box->setColor(Color3B::GRAY);
    }
    else
    {
        box->place();
        box->setColor(Color3B::WHITE);
    }
    m_SharedGameBoxes->m_fightButtonBoxAuth = isAuth;
}
bool GameBoxes::getFightButtonBoxAuth()
{
    return m_SharedGameBoxes->m_fightButtonBoxAuth;
}
//RUMBLE BOXES.

bool GameBoxes::startRumbleBox(std::vector<int> boxesTagList)
{
    for(int b = 0; b < boxesTagList.size(); b++)
    {
        auto box = MainObject::getBoxByTag(boxesTagList[b]);
        if(box)
        {
            box->startRumble();
        }
    }
    return true;
}
bool GameBoxes::stopRumbleBox()
{
    auto sceneChildren = MainObject::getMainLayer()->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto box = dynamic_cast<Box*>(*scIt);
        if(box && box->getIsRumble())
            box->stopRumble();
    }
    return true;
}

//-------------------------------ACTION BOXES----------------------------------
void GameBoxes::setSelectActionBoxes(std::vector<cocos2d::Vec3> selectedActionBoxes)
{
    setUnselectActionBoxes();
    
    for(int t = 0; t < selectedActionBoxes.size(); t++)
    {
        auto box = MainObject::getBoxByTag(selectedActionBoxes[t].y);
        if(box)
            box->setActionUI(selectedActionBoxes[t].z, GameDirector::getScene()->getIsPlayerTurn());
    }
}

void GameBoxes::setUnselectActionBoxes()
{
    auto sceneChildren = MainObject::getMainLayer()->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto box = dynamic_cast<Box*>(*scIt);
        if(box && box->getIsActionUI())
            box->resetActionUI();
    }
}

//-------------------------UNSELECT ALL----------------------
void GameBoxes::unselectAll()
{
    auto nodeList = MainObject::getMainLayer()->getChildren();
    std::vector<Node*>::iterator nlIt;
    for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
    {
        auto box = dynamic_cast<Box*>(*nlIt);
        if(box)
        {
            setBoxUnselect(box->getTag());
            if(box->getIsActionUI())
                box->resetActionUI();
        }
    }
}
