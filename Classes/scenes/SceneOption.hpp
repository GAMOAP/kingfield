//
//  SceneOption.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 03/10/2019.
//

#ifndef SceneOption_hpp
#define SceneOption_hpp

#include "cocos2d.h"

#include "KFScene.hpp"
#include "KFNode.hpp"

class SceneOption : public KFScene
{
public:
    static SceneOption* setScene(std::string oldScene, int tag);
    
    virtual bool init();
    virtual void addToStage();
    virtual void removeToStage();
    
    virtual bool touchBox(int tag);
    virtual bool unTouchBox(int tag);
    
private:
    void initFocus();
    void resetFocus();
    
    void addObjects();
    void removeObjects();
    
protected:
    const float m_fadeTime = 0.3;
    time_t m_time;
    
    KFNode* m_focusedNode = nullptr;
    cocos2d::Node* m_cloneNode = nullptr;
    cocos2d::Vec2 m_cloneNodeLC;
    float m_cloneNodeScale;
    
    std::string m_oldScene;
    
    bool m_removeAuth = true;
    bool m_removing = false;
    
    cocos2d::Sprite* m_mask = nullptr;
    cocos2d::Sequence* m_maskSeq = nullptr;
    cocos2d::Sequence* m_nodeSeq = nullptr;
};

static SceneOption* m_SharedSceneOption = nullptr;

#endif /* SceneOption_hpp */
