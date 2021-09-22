//
//  SceneIntro.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 25/09/2019.
//

#ifndef SceneIntro_hpp
#define SceneIntro_hpp

#include "KFScene.hpp"

class SceneIntro : public KFScene
{
public:
    static SceneIntro* setScene();
    
    virtual bool init();
    virtual void addToStage();
    virtual void removeToStage();
    
    virtual bool allNodeIsIn();
    virtual bool appConnected();
    
    virtual bool touchBox(int tag);
    virtual bool unTouchBox(int tag);
    
private:
    
    
protected:
    bool m_removeAuth;
    
};

static SceneIntro* m_SharedSceneIntro = nullptr;

#endif /* SceneIntro_hpp */
