//
//  MainSounds.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 08/07/2021.
//

#ifndef MainSounds_hpp
#define MainSounds_hpp

#include "cocos2d.h"

class MainSounds //: public cocos2d::Node
{
public:
    
    /**
     * @brief get the instance of MainSouds or create and init it, if don't exist.
    */
    static MainSounds* getInstance();
    bool init();
    
    /**
     * @brief play game music
     * @param theme music theme name.
    */
    static void playMusic(std::string theme);
    
    /**
     * @brief play game soud effect.
     * @param theme sound effect name.
     * @param volume sound volume.
    */
    static void playSound(std::string sound, float volume = 1.0f);
    
    /**
     * @brief play box  soud effect.
     * @param mouvement type of box animation.
     * @param boxTag box's KFNode tag
    */
    static void playBox(std::string mouvement, int boxTag);
    
    /**
     * @brief play char  soud effect.
     * @param saction type of char action.
     * @param charNbr box's KFNode tag
    */
    static void playChar(std::string action, int charNbr);
    
    /**
     * @brief preload all game sound.
    */
    static void preLoad();
    
    /**
     * @brief get and set play sound box authorization.
    */
    void setBoxSoundAuth(bool Auth);
    bool getBoxSoundAuth();
    
    
private:
    
    //play sound box authorization.
    bool m_boxSoundAuth;
};

//unique instance of MainSouds.
static MainSounds* m_SharedMainSounds = nullptr;

#endif /* MainSounds_hpp */
