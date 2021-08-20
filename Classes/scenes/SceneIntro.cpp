//
//  SceneIntro.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 25/09/2019.
//

#include "SceneIntro.hpp"

#include "Constants.h"

#include "MainMultiPlayer.hpp"

#include "MainSounds.hpp"

#include "GameDirector.hpp"
#include "GameInfoLayer.hpp"
#include "GameBoxes.hpp"
#include "GameCharacters.hpp"
#include "GameCards.hpp"

#include <iostream>

USING_NS_CC;

SceneIntro* SceneIntro::setScene()
{
    if (!m_SharedSceneIntro)
    {
        m_SharedSceneIntro = new (std::nothrow) SceneIntro;
        m_SharedSceneIntro->init();
    }
    return m_SharedSceneIntro;
}

bool SceneIntro::init()
{
    setName("intro");;
    
    m_removeAuth = false;
    
    return true;
}

void SceneIntro::addToStage()
{
    GameBoxes::setBoxes();
    GameCharacters::setCharacters(0);
    
    auto gameCards = GameCards::getInstance();
    gameCards->removeDeck();
    gameCards->removeSheet();
    gameCards->removeLibrary();
}

void SceneIntro::removeToStage()
{
    GameInfoLayer::removeIntroTitle();
    GameDirector::setScene("barrack");
    
    std::string breed = GameDirector::getKingBreed();
    MainSounds::playSound("box_" + breed + "_remove");
}


bool SceneIntro::allNodeIsIn()
{
    //start multiplayer if activate in constant.h
    if(!MULTI_PLAYER_ON)
    {
        m_removeAuth = true;
    }
    else
    {
        MainMultiPlayer::connect();
    }
    
    //add Tittle
    GameInfoLayer::addIntroTitle();
    
    return true;
}

bool SceneIntro::appConnected()
{
    GameInfoLayer::removeConnectSprite();
    m_removeAuth = true;
    return true;
}


bool SceneIntro::unTouchBox(int tag)
{
    if(m_removeAuth)
    {
        removeToStage();
    }
    
    return true;
}
