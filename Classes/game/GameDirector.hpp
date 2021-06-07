//
//  GameDirector.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 28/04/2019.
//

#ifndef GameDirector_hpp
#define GameDirector_hpp

#include "cocos2d.h"
#include "KFScene.hpp"

class GameDirector
{
public:
    
    static GameDirector* getInstance();
    
    static void setScene(std::string scene, int tag = NAN);
    static KFScene* getScene();
    
    //EVENT
    static bool allNodeIsIn();
    
    static bool appConnected();
    static bool appConnecting();
    
    //FIGHT EVENT
    static bool startFight(int teamNumber);
    static bool stopFight(bool isWin);
    static bool endTurn();
    
    
    static void setActionInProgress(bool isInProgress);
    static bool getActionInProgress();
    
    //TOUCH EVENT
    static void touchBox(int tag);
    static void unTouchBox(int tag);
    static void cancelTouchBox(int tag);
    
    static void longTouchBox(int tag);
    
    //KING BREED
    static void setKingBreed(std::string kingBreed);
    static std::string getKingBreed();
    static std::string getEnemyKingBreed();
    
private:
    bool init();
    
protected:
    KFScene* m_KFScene = nullptr;
    bool m_touchLongAuth = false;
    
    bool m_actionInProgress;
};

static GameDirector* m_SharedGameDirector = nullptr;
static std::string m_kingBreed;
static std::string m_enemyKingBreed = "time";

#endif /* GameDirector_hpp */
