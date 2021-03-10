//
//  MainScene.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 28/04/2019.
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"


#include "MainGrid.hpp"
#include "MainUser.hpp"
#include "MainObject.hpp"
#include "MainStuff.hpp"
#include "MainMultiPlayer.hpp"

#include "MainDirector.hpp"

class MainScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);
    
    CREATE_FUNC(MainScene);
    
private:
    
    MainGrid* m_mainGrid = nullptr;
    MainObject* m_mainObject = nullptr;
    MainStuff* m_mainStuff = nullptr;
    
    virtual void onTouchLong(int boxTag);
    virtual void cancelTouchLong();
    
    cocos2d::Sequence* m_longTouch = nullptr;
    int m_touchBoxTag;
};

#endif /* MainScene_hpp */
