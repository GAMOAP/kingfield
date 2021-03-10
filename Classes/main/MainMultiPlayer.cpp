//
//  MainMultiPlayer.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 02/09/2019.
//

#include "MainMultiPlayer.hpp"

#include "Constants.h"

#include "MainUser.hpp"
#include "MainObject.hpp"
#include "MainAction.hpp"

#include "MainDirector.hpp"
#include "MainInfoLayer.hpp"

#include "GameCharacters.hpp"

#include <iostream>

USING_NS_CC;

MainMultiPlayer* MainMultiPlayer::getInstance()
{
    if (!m_SharedMainMultiPlayer)
    {
        m_SharedMainMultiPlayer = new (std::nothrow) MainMultiPlayer;
        m_SharedMainMultiPlayer->init();
    }
    return m_SharedMainMultiPlayer;
}
bool MainMultiPlayer::init()
{
    MainObject::getInfoLayer()->addChild(this);
    this->scheduleUpdate();
    return true;
}

void MainMultiPlayer::update(float dt)
{
    
}

//-------------------------PUBLIC FUNCTION--------------------------------

void MainMultiPlayer::connect()
{
    auto multiPlayer = MainMultiPlayer::getInstance();
    multiPlayer->m_isFirstLaunch = true;
    multiPlayer->m_userName = MainUser::getUserName();
    
    multiPlayer->connectToAppWarp(multiPlayer);
}
void MainMultiPlayer::disconnect()
{
    if(c_testNetOn)printf("\ndisconnectToAppWarp");
    AppWarp::Client::getInstance()->disconnect();
}

void MainMultiPlayer::joinRoom()
{
    AppWarp::Client::getInstance()->joinRoomInUserRange(0, 1, true);
}
void MainMultiPlayer::leaveRoom()
{
    AppWarp::Client::getInstance()->leaveRoom(m_SharedMainMultiPlayer->m_roomId);
}


//-------------------------CONNECT APPWARP-------------------------------
void MainMultiPlayer::connectToAppWarp(Ref* pSender)
{
    if(c_testNetOn)printf("\nconnectToAppWarp");
    m_isConnected = false;
    AppWarp::Client *warpClientRef;
    if (m_isFirstLaunch)
    {
        m_isFirstLaunch = !m_isFirstLaunch;
        AppWarp::Client::initialize(m_appKey, m_secretKey);
        warpClientRef = AppWarp::Client::getInstance();
        //warpClientRef->setGeo("sgp");
        warpClientRef->setRecoveryAllowance(30);
        warpClientRef->setConnectionRequestListener(this);
        warpClientRef->setNotificationListener(this);
        warpClientRef->setRoomRequestListener(this);
        warpClientRef->setZoneRequestListener(this);
        warpClientRef->connect(m_userName);
        scheduleConnect();
    }
    else
    {
        AppWarp::Client::getInstance()->connect(m_userName);
    }
}

//-------------------------ON CONNECT-------------------------------
void MainMultiPlayer::onConnectDone(int res, int reasonCode)
{
    if (res==AppWarp::ResultCode::success)
    {
        unscheduleRecover();
        unscheduleConnect();
        m_isConnected = true;
        if(c_testNetOn)printf("\nonConnectDone .. SUCCESS..session=%d\n",AppWarp::AppWarpSessionID);
        MainDirector::appConnected();
    }
    else if (res==AppWarp::ResultCode::auth_error)
    {
        unscheduleRecover();
        if(c_testNetOn)printf("\nonConnectDone .. Failed with auth_error and reasonCode = %d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
    }
    else if (res==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        if(c_testNetOn)printf("\nonConnectDone .. SUCCESS with success_recovered..session=%d\n",AppWarp::AppWarpSessionID);
        MainDirector::appConnected();
    }
    else if (res==AppWarp::ResultCode::connection_error_recoverable)
    {
        scheduleRecover();
        if(c_testNetOn)printf("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (res==AppWarp::ResultCode::bad_request)
    {
        disconnect();
        if(c_testNetOn)printf("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else
    {
        disconnect();
        if(c_testNetOn)printf("\nonConnectDone .. FAILED with reasonCode=%d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
    }
}

//-----------------------------SCHEDULE-------------------------------

void MainMultiPlayer::scheduleConnect()
{
    if(c_testNetOn)printf("\nscheduleConnect");
    if (!this->isScheduled(schedule_selector(MainMultiPlayer::connect)) && !m_isConnected)
    {
        this->schedule(schedule_selector(MainMultiPlayer::connect), 20.0f);
        MainDirector::appConnecting();
    }
}
void MainMultiPlayer::unscheduleConnect()
{
    if(c_testNetOn)printf("\nunscheduleConnect");
    unschedule(schedule_selector(MainMultiPlayer::connect));
}
void MainMultiPlayer::connect(float dt)
{
    if(c_testNetOn)printf("\nconnect");
    AppWarp::Client::getInstance()->connect(m_userName);
}

void MainMultiPlayer::scheduleRecover()
{
    if(c_testNetOn)printf("\nscheduleRecover");
    if (!this->isScheduled(schedule_selector(MainMultiPlayer::recover)))
    {
        this->schedule(schedule_selector(MainMultiPlayer::recover), 5.0f);
        MainDirector::appConnecting();
    }
}
void MainMultiPlayer::unscheduleRecover()
{
    if(c_testNetOn)printf("\nunscheduleRecover");
    unschedule(schedule_selector(MainMultiPlayer::recover));
}
void MainMultiPlayer::recover(float dt)
{
    if(c_testNetOn)printf("\nrecover");
    AppWarp::Client::getInstance()->recoverConnection();
}

//--------------------------CONNECT ROOM-----------------------------
void MainMultiPlayer::onCreateRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        if(c_testNetOn)printf("\nonCreateRoomDone .. SUCCESS");
        AppWarp::Client::getInstance()->joinRoom(revent.roomId);
    }
    else
        if(c_testNetOn)printf("\nonCreateRoomDone .. FAILED... error==%d",revent.result);
}
void MainMultiPlayer::onDeleteRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        if(c_testNetOn)printf("\nonDeleteRoomDone .. SUCCESS");
        //GameDirector::GameOver(bool isWin);
    }
    else
        if(c_testNetOn)printf("\nonDeleteRoomDone .. FAILED... error==%d",revent.result);
}

void MainMultiPlayer::onJoinRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        std::string roomId = revent.roomId;
        if(c_testNetOn)printf("\nonJoinRoomDone .. SUCCESS..room_id=%s",roomId.c_str());
        AppWarp::Client::getInstance()->subscribeRoom(roomId);
    }
    else
    {
        if(c_testNetOn)printf("\nonJoinRoomDone .. FAILED");
        AppWarp::Client::getInstance()->createRoom("fightRoom", "a20whe", 2);
    }
}

void MainMultiPlayer::onSubscribeRoomDone(AppWarp::room revent)
{
    if (revent.result==0)
    {
        if(c_testNetOn)printf("\nonSubscribeRoomDone .. SUCCESS");
        AppWarp::Client::getInstance()->getLiveRoomInfo(revent.roomId);
        m_roomId = revent.roomId;
    }
    else
        if(c_testNetOn)printf("\nonSubscribeRoomDone .. FAILED... error==%d",revent.result);
}
void MainMultiPlayer::onGetLiveRoomInfoDone(AppWarp::liveroom levent)
{
    if (levent.result==0)
    {
        std::vector<std::string> users = levent.users;
        int usersSize = users.size();
        if(c_testNetOn)printf("\nonGetLiveRoomInfoDone .. SUCCESS... userNbr=%i",usersSize);
        if(usersSize >= 2)
        {
            int userNbr;
            for (int u = 0; u < usersSize; u++)
            {
                std::string userName = users[u];
                if(userName == m_userName)
                    userNbr = u;
            }
            sendStartGameData(userNbr);
        }
    }
    else
        if(c_testNetOn)printf("\nonGetLiveRoomInfoDone .. FAILED... error==%d",levent.result);
}

void MainMultiPlayer::onUserJoinedRoom(AppWarp::room revent, std::string userName)
{
    if(c_testNetOn)printf("\nonUserJoinRoom .. SUCCESS .. user=%s", userName.c_str());
    std::string roomId = revent.roomId;
    if(roomId == m_roomId)
        AppWarp::Client::getInstance()->getLiveRoomInfo(revent.roomId);
}

void MainMultiPlayer::onUserLeftRoom(AppWarp::room revent, std::string userName)
{
    if(c_testNetOn)printf("\nonUserLeftRoom .. SUCCESS .. room_id=%s user=%s", revent.roomId.c_str(), userName.c_str());
    AppWarp::Client::getInstance()->deleteRoom(revent.roomId);
    MainDirector::stopFight(userName != m_userName);
}

//-----------------------SEND RECEIVE DATA--------------------------
void MainMultiPlayer::sendStartGameData(int userNbr)
{
    std::string userTeamStr = MainUser::getUserTeamStr();
    AppWarp::Client::getInstance()->sendChat(userTeamStr);
    
    auto teamReceived = EventListenerCustom::create("TEAM_RECEIVED", [=](EventCustom* event)
    {
        MainDirector::startFight(userNbr);
        _eventDispatcher->removeCustomEventListeners("TEAM_RECEIVED");
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(teamReceived, this);
}
void MainMultiPlayer::sendCharacterActionData(int charNbr, int cardNbr, int actionBoxTag)
{
    std::stringstream actionData;
    actionData <<"p"<< charNbr << "c" <<cardNbr << "a" << actionBoxTag;
    AppWarp::Client::getInstance()->sendChat(actionData.str());
}
void MainMultiPlayer::echo(std::string message)
{
    _eventDispatcher->dispatchCustomEvent(message);
}

//-------------------------CHAT PROTOCOL-------------------------------
void MainMultiPlayer::onChatReceived(AppWarp::chat chatEvent)
{
    if(chatEvent.sender != m_userName)
    {
        if(c_testNetOn)printf("\nonChatReceived..");
        std::string zstr = chatEvent.chat.substr(0,1);
        if(zstr == "c")
        {
            if(c_testNetOn)printf("userTeamStr");
            std::string opposingTeamStr = chatEvent.chat;
            if(MainUser::setOpposingTeam(opposingTeamStr))
            {
                std::string echoMessage = "TEAM_RECEIVED";
                AppWarp::Client::getInstance()->sendChat(echoMessage.c_str());
            }
        }
        else if(zstr == "p")
        {
            if(c_testNetOn)printf("characterActionDataStr");
            std::string actionData = chatEvent.chat;
            int charNbr = stoi(actionData.substr(actionData.find('p') + 1, 1));
            int cardNbr = stoi(actionData.substr(actionData.find('c') + 1, 1));
            int boxTag = stoi(actionData.substr(actionData.find('a') + 1, 2));
                               
            auto actionSequence = MainAction::getEnemyActionSequence(charNbr, cardNbr, boxTag);
            GameCharacters::setAction(actionSequence);
            MainDirector::setActionInProgress(true);
        }
        else if(zstr == "t")
        {
            if(c_testNetOn)printf("void");
        }
        else
            echo(chatEvent.chat);
    }
}
