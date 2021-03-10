//
//  MainDirector.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 10/03/2021.
//

#ifndef MainDirector_hpp
#define MainDirector_hpp

#include "cocos2d.h"
#include "KFScene.hpp"

class MainDirector
{
public:
    
    static MainDirector* getInstance();
    
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
    
private:
    bool init();
    
protected:
    KFScene* m_KFScene = nullptr;
    bool m_touchLongAuth = false;
    
    bool m_actionInProgress;
};

static MainDirector* m_SharedMainDirector = nullptr;
static std::string m_kingBreed;

#endif /* MainDirector_hpp */
