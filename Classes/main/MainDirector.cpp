//
//  MainDirector.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 10/03/2021.
//

#include "MainDirector.hpp"

#include "MainObject.hpp"

#include "GameBoxes.hpp"
#include "GameCharacters.hpp"
#include "GameCards.hpp"

#include "SceneIntro.hpp"
#include "SceneBarrack.hpp"
#include "SceneFight.hpp"
#include "SceneOption.hpp"

#include <iostream>

USING_NS_CC;

MainDirector* MainDirector::getInstance()
{
    if (!m_SharedMainDirector)
    {
        m_SharedMainDirector = new (std::nothrow) MainDirector;
        m_SharedMainDirector->init();
    }
    return m_SharedMainDirector;
}

bool MainDirector::init()
{
    //init game instance
    GameBoxes::getInstance();
    GameCharacters::getInstance();
    GameCards::getInstance();
    
    m_kingBreed = "sun";
    
    return true;
}

void MainDirector::setScene(std::string scene, int tag)
{
    const auto oldScene = m_SharedMainDirector->m_KFScene;
    
    if(scene == "intro")
        m_SharedMainDirector->m_KFScene = SceneIntro::setScene();
    if(scene == "barrack")
        m_SharedMainDirector->m_KFScene = SceneBarrack::setScene();
    if(scene == "fight")
        m_SharedMainDirector->m_KFScene = SceneFight::setScene();
    if(scene == "option")
    {
        std::string oldScene = m_SharedMainDirector->m_KFScene->getName();
        m_SharedMainDirector->m_KFScene = SceneOption::setScene(oldScene, tag);
    }
    
    
    if(oldScene && oldScene->getName() == "option")
    {
        
    }
    else
    {
        m_SharedMainDirector->m_actionInProgress = false;
        m_SharedMainDirector->m_KFScene->addToStage();
    }
}

//-----------------------------GAME EVENT--------------------------------
bool MainDirector::allNodeIsIn()
{
    m_SharedMainDirector->m_KFScene->allNodeIsIn();
    return true;
}
bool MainDirector::appConnected()
{
    m_SharedMainDirector->m_KFScene->appConnected();
    return true;
}
bool MainDirector::appConnecting()
{
    m_SharedMainDirector->m_KFScene->appConnecting();
    return true;
}
//-----------------------------FIGHT EVENT--------------------------------
bool MainDirector::startFight(int teamNumber)
{
    if(m_SharedMainDirector->m_KFScene->getName() == "fight")
        SceneFight::startFight(teamNumber);
    return true;
}
bool MainDirector::stopFight(bool isWin)
{
    if(m_SharedMainDirector->m_KFScene->getName() == "fight")
        SceneFight::stopFight(isWin);
    return true;
}
bool MainDirector::endTurn()
{
    if(m_SharedMainDirector->m_KFScene->getName() == "fight")
        SceneFight::endTurn();
    return true;
}
void MainDirector::setActionInProgress(bool isInProgress){m_SharedMainDirector->m_actionInProgress = isInProgress;}
bool MainDirector::getActionInProgress(){return m_SharedMainDirector->m_actionInProgress;}

//------------------------------TOUCH EVENT-------------------------------
void MainDirector::touchBox(int tag)
{
    auto mainObject = MainObject::getInstance();
    if(!mainObject->isNodesMove() &&
       mainObject->isAllNodeIsIn() &&
       !m_SharedMainDirector->m_actionInProgress)
    {
        m_SharedMainDirector->m_KFScene->touchBox(tag);
        
        //Authorize touch long.
        if(m_SharedMainDirector->m_KFScene->getName() != "option")
            m_SharedMainDirector->m_touchLongAuth = true;
        else
            m_SharedMainDirector->m_touchLongAuth = false;
    }
}

void MainDirector::unTouchBox(int tag)
{
    auto mainObject = MainObject::getInstance();
    if(mainObject->isAllNodeIsIn())
    {
        m_SharedMainDirector->m_KFScene->unTouchBox(tag);
    }
}

void MainDirector::cancelTouchBox(int tag)
{
    m_SharedMainDirector->m_KFScene->cancelTouchBox(tag);
}

void MainDirector::longTouchBox(int tag)
{
    auto mainObject = MainObject::getInstance();
    
    if(!mainObject->isNodesMove() && mainObject->isAllNodeIsIn() && m_SharedMainDirector->m_touchLongAuth)
        m_SharedMainDirector->m_KFScene->longTouchBox(tag);
}


//----------------------------------KING BREED-----------------------------------
void MainDirector::setKingBreed(std::string kingBreed)
{
    m_kingBreed = kingBreed;
}
KFScene* MainDirector::getScene()
{
    return m_SharedMainDirector->m_KFScene;
}
std::string MainDirector::getKingBreed()
{
    return m_kingBreed;
}

