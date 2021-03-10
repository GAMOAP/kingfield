//
//  MainMultiPlayer.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 02/09/2019.
//

#ifndef MainMultiPlayer_hpp
#define MainMultiPlayer_hpp

#include "appwarp.h"

class MainMultiPlayer : public cocos2d::Node, public AppWarp::ConnectionRequestListener, public AppWarp::RoomRequestListener, public AppWarp::NotificationListener, public AppWarp::ZoneRequestListener, public AppWarp::LobbyRequestListener
{
public:
    static MainMultiPlayer* getInstance();
    
    static void connect();
    static void disconnect();
    
    static void joinRoom();
    static void leaveRoom();
    
    void update(float dt);
    
    //CHAT----
    static void sendCharacterActionData(int charNbr, int cardNbr, int actionBoxTag);
    
private:
    bool init();
    
//CONNECT------
    void connectToAppWarp(Ref* pSender);
    
    void onConnectDone(int res, int reasonCode);
    
    void scheduleConnect();
    void unscheduleConnect();
    void connect(float dt);
    
    void scheduleRecover();
    void unscheduleRecover();
    void recover(float dt);
//ROOM--------
    void onCreateRoomDone(AppWarp::room revent);
    void onDeleteRoomDone(AppWarp::room revent);
    
    void onJoinRoomDone(AppWarp::room revent);
    void onSubscribeRoomDone(AppWarp::room revent);
    
    void onGetLiveRoomInfoDone(AppWarp::liveroom levent);
    
    void onUserJoinedRoom(AppWarp::room revent, std::string userName);
    void onUserLeftRoom(AppWarp::room revent, std::string userName);
    
//CHAT-----------
    void sendStartGameData(int userNbr);
    void characterAction(std::string chat);
    void echo(std::string message);
    
    void onChatReceived(AppWarp::chat chatevent);
      
protected:
    const std::string m_appKey = "8c778a3575f224c9953162946f5472fe2ec76c8c789010fb64291093204931fe";
    const std::string m_secretKey = "7a9e9f5391bf37e58095223c47992f77e91d23d6754ff9c05a867c1f46f8051f";
    
    bool m_isConnected;
    
    float m_delayReconnect = (5);
    bool m_reconnecting;
    
    bool m_isFirstLaunch;
    
    std::string m_roomId;
    std::string m_userName;
    
    bool m_isEcho;
};

static MainMultiPlayer* m_SharedMainMultiPlayer = nullptr;

#endif /* MainMultiPlayer_hpp */
