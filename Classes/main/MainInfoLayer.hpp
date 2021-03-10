//
//  MainInfoLayer.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 10/03/2021.
//

#ifndef MainInfoLayer_hpp
#define MainInfoLayer_hpp

#include "cocos2d.h"
#include "KFSprite.hpp"

class MainInfoLayer
{
public:
    static MainInfoLayer* getInstance();
    
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

static MainInfoLayer* m_SharedMainInfoLayer = nullptr;

#endif /* MainInfoLayer_hpp */
