//
//  GameInfoLayer.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/09/2019.
//

#ifndef GameInfoLayer_hpp
#define GameInfoLayer_hpp

#include "cocos2d.h"

class GameInfoLayer
{
public:
    static GameInfoLayer* getInstance();
    
    static void addIntroTitle();
    static void removeIntroTitle();
    
    static void addConnectSprite();
    static void removeConnectSprite();
    
    static void addSocialLogo();
    static void removeSocialLogo();
    
    static void addInfoLogo(std::string name, int line, int collumn);
    
    
    
private:
    virtual bool init();
    
    void initConnectSprite(bool addRemove);
    void initIntroTitle(bool addRemove);
    void initSocialLogo(bool addRemove);
    void initInfoLogo(std::string name, int line, int collumn);
    
protected:
    cocos2d::Node* m_infoLayer = nullptr;
    
    cocos2d::Sprite* m_introLogo = nullptr;
    cocos2d::Sprite* m_spriteConnect = nullptr;
    cocos2d::Sprite* m_fightLogo = nullptr;
    
    cocos2d::Sprite* m_githubLogo = nullptr;
    cocos2d::Sprite* m_twitterLogo = nullptr;
    
};

static GameInfoLayer* m_SharedGameInfoLayer = nullptr;

#endif /* GameInfoLayer_hpp */
