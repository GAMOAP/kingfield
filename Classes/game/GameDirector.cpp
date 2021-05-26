//
//  GameDirector.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 28/04/2019.
//

#include "GameDirector.hpp"

#include "MainObject.hpp"
#include "MainStuff.hpp"

#include "GameBoxes.hpp"
#include "GameCharacters.hpp"
#include "GameCards.hpp"

#include "SceneIntro.hpp"
#include "SceneBarrack.hpp"
#include "SceneFight.hpp"
#include "SceneOption.hpp"

#include <iostream>

USING_NS_CC;

GameDirector* GameDirector::getInstance()
{
    if (!m_SharedGameDirector)
    {
        m_SharedGameDirector = new (std::nothrow) GameDirector;
        m_SharedGameDirector->init();
    }
    return m_SharedGameDirector;
}

bool GameDirector::init()
{
    //init game instance
    GameBoxes::getInstance();
    GameCharacters::getInstance();
    GameCards::getInstance();
    
    m_kingBreed = MainStuff::getTeamKarma(0);
    
    return true;
}

void GameDirector::setScene(std::string scene, int tag)
{
    const auto oldScene = m_SharedGameDirector->m_KFScene;
    
    if(scene == "intro")
        m_SharedGameDirector->m_KFScene = SceneIntro::setScene();
    if(scene == "barrack")
        m_SharedGameDirector->m_KFScene = SceneBarrack::setScene();
    if(scene == "fight")
        m_SharedGameDirector->m_KFScene = SceneFight::setScene();
    if(scene == "option")
    {
        std::string oldScene = m_SharedGameDirector->m_KFScene->getName();
        m_SharedGameDirector->m_KFScene = SceneOption::setScene(oldScene, tag);
    }
    
    
    if(oldScene && oldScene->getName() == "option")
    {
        
    }
    else
    {
        m_SharedGameDirector->m_actionInProgress = false;
        m_SharedGameDirector->m_KFScene->addToStage();
    }
}

//-----------------------------GAME EVENT--------------------------------
bool GameDirector::allNodeIsIn()
{
    m_SharedGameDirector->m_KFScene->allNodeIsIn();
    return true;
}
bool GameDirector::appConnected()
{
    m_SharedGameDirector->m_KFScene->appConnected();
    return true;
}
bool GameDirector::appConnecting()
{
    m_SharedGameDirector->m_KFScene->appConnecting();
    return true;
}
//-----------------------------FIGHT EVENT--------------------------------
bool GameDirector::startFight(int teamNumber)
{
    if(m_SharedGameDirector->m_KFScene->getName() == "fight")
        SceneFight::startFight(teamNumber);
    return true;
}
bool GameDirector::stopFight(bool isWin)
{
    if(m_SharedGameDirector->m_KFScene->getName() == "fight")
        SceneFight::stopFight(isWin);
    return true;
}
bool GameDirector::endTurn()
{
    if(m_SharedGameDirector->m_KFScene->getName() == "fight")
        SceneFight::endTurn();
    return true;
}
void GameDirector::setActionInProgress(bool isInProgress){m_SharedGameDirector->m_actionInProgress = isInProgress;}
bool GameDirector::getActionInProgress(){return m_SharedGameDirector->m_actionInProgress;}

//------------------------------TOUCH EVENT-------------------------------
void GameDirector::touchBox(int tag)
{
    auto mainObject = MainObject::getInstance();
    if(!mainObject->isNodesMove() &&
       mainObject->isAllNodeIsIn() &&
       !m_SharedGameDirector->m_actionInProgress)
    {
        m_SharedGameDirector->m_KFScene->touchBox(tag);
        
        //Authorize touch long.
        if(m_SharedGameDirector->m_KFScene->getName() != "option")
            m_SharedGameDirector->m_touchLongAuth = true;
        else
            m_SharedGameDirector->m_touchLongAuth = false;
    }
}

void GameDirector::unTouchBox(int tag)
{
    auto mainObject = MainObject::getInstance();
    if(mainObject->isAllNodeIsIn())
    {
        m_SharedGameDirector->m_KFScene->unTouchBox(tag);
    }
}

void GameDirector::cancelTouchBox(int tag)
{
    m_SharedGameDirector->m_KFScene->cancelTouchBox(tag);
}

void GameDirector::longTouchBox(int tag)
{
    auto mainObject = MainObject::getInstance();
    
    if(!mainObject->isNodesMove() && mainObject->isAllNodeIsIn() && m_SharedGameDirector->m_touchLongAuth)
        m_SharedGameDirector->m_KFScene->longTouchBox(tag);
}


//----------------------------------KING BREED-----------------------------------
void GameDirector::setKingBreed(std::string kingBreed)
{
    m_kingBreed = kingBreed;
    const auto charKing = GameCharacters::getKingFriend();
    charKing->setFlag();
    GameBoxes::setBoxes();
}
KFScene* GameDirector::getScene()
{
    return m_SharedGameDirector->m_KFScene;
}
std::string GameDirector::getKingBreed()
{
    return m_kingBreed;
}
