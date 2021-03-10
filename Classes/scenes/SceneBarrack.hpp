//
//  SceneBarrack.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 01/10/2019.
//

#ifndef SceneBarrack_hpp
#define SceneBarrack_hpp

#include "KFScene.hpp"

#include "cocos2d.h"

class SceneBarrack : public KFScene
{
public:
    static SceneBarrack* setScene();
    
    virtual bool init();
    virtual void addToStage();
    virtual void removeToStage();
    
    virtual bool allNodeIsIn();
    
    virtual bool touchBox(int tag);
    virtual bool unTouchBox(int tag);
    virtual bool cancelTouchBox(int tag);
    
    virtual bool longTouchBox(int tag);
    
private:
    void addLibraryButton();
    void removeLibraryButton();
    void pushLibraryButton(int tag);
    void cancelPushLibraryButton();
    
protected:
    const int m_fightButtonTag = 23;
    const int m_libraryLeftButtonTag = 32;
    const int m_libraryRightButtonTag = 34;
    
    cocos2d::Sprite* m_libraryButtonLeft = nullptr;
    cocos2d::Sprite* m_libraryButtonRight = nullptr;
    
};

static SceneBarrack* m_SharedSceneBarrack = nullptr;

#endif /* SceneBarrack_hpp */
