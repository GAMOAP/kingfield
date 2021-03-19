//
//  GameInfoLayer.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/09/2019.
//

#ifndef GameInfoLayer_hpp
#define GameInfoLayer_hpp

#include "cocos2d.h"
#include "KFSprite.hpp"

class GameInfoLayer
{
public:
    static GameInfoLayer* getInstance();
    
    static void addIntroTitle();
    static void removeIntroTitle();
    
    static void addConnectSprite();
    static void removeConnectSprite();
    
private:
    virtual bool init();
    
    void initConnectSprite(bool addRemove);
    void initIntroTitle(bool addRemove);
    
protected:
    cocos2d::Node* m_infoLayer = nullptr;
    
    cocos2d::Sprite* m_introLogo = nullptr;
    cocos2d::Sprite* m_spriteConnect = nullptr;
    
};

static GameInfoLayer* m_SharedGameInfoLayer = nullptr;

#endif /* GameInfoLayer_hpp */