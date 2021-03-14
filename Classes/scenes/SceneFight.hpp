//
//  SceneFight.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 03/10/2019.
//

#ifndef SceneFight_hpp
#define SceneFight_hpp

#include "KFScene.hpp"

class SceneFight : public KFScene
{
public:
    static SceneFight* setScene();
    
    virtual bool init();
    virtual void addToStage();
    virtual void removeToStage();
    
    virtual bool allNodeIsIn();
    
    static bool startFight(int teamNumber);
    static bool stopFight(bool isWin);
    
    static bool endTurn();
    
    virtual bool touchBox(int tag);
    virtual bool unTouchBox(int tag);
    virtual bool cancelTouchBox(int tag);
    
    virtual bool longTouchBox(int tag);
    
    virtual bool getIsPlayerTurn();
    virtual bool getIsTeamTurn(int charNbr);
    
private:
    bool setActionBoxTags();
    
    bool getIsBoardTag(int tag);
    
    
protected:
    int m_teamNumber;
    int m_turnNumber;
    
    const std::vector<int> m_fightFieldTag = {30,31,32,33,34,36,40,41,42,43,44,45,46,50,51,52,53,54,55,56,60,61,62,63,64,65,66,70,71,72,73,74,75,76};
    const std::vector<int> m_boardTag = {21,22,23,24,25,31,32,33,34,35,41,42,43,44,45,51,52,53,54,55,61,62,63,64,65};
};

static SceneFight* m_SharedSceneFight = nullptr;


#endif /* SceneFight_hpp */
