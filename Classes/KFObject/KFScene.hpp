//
//  KFScene.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 24/09/2019.
//

#ifndef KFScene_hpp
#define KFScene_hpp

#include "cocos2d.h"

class KFScene : public cocos2d::Node
{
public:
    virtual bool init() = 0;
    virtual void addToStage();
    virtual void removeToStage();
    
    virtual bool allNodeIsIn();
    
    virtual bool appConnected();
    virtual bool appConnecting();
    
    virtual bool touchBox(int tag);
    virtual bool unTouchBox(int tag);
    virtual bool cancelTouchBox(int tag);
    
    virtual bool longTouchBox(int tag);
    
    virtual void setName(std::string sceneName);
    virtual std::string getName();
    
    virtual bool getIsPlayerTurn();
    virtual bool getIsTeamTurn(int charNbr);
    virtual int getTurnNumber();
       
    virtual bool getIsUsedBox(int tag);
    virtual void setTouchObject(int tag);
    virtual void cancelTouchObject();
    
private:
    
    
    
    
protected:
    int m_touchedObjectTag;
    
    int m_touchedBox;
    
    std::string m_name;
};

#endif /* KFScene_hpp */
