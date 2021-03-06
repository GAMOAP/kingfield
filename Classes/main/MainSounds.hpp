//
//  MainSounds.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 08/07/2021.
//

#ifndef MainSounds_hpp
#define MainSounds_hpp

#include "cocos2d.h"

class MainSounds : public cocos2d::Node
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
     * @brief play game  soud effect.
     * @param sound game sound name.
     * @param boxTag box's KFNode tag
    */
    static void playGame(std::string sound);
    
    /**
     * @brief play box  soud effect.
     * @param mouvement type of box animation.
     * @param boxTag box's KFNode tag
    */
    static void playBox(std::string mouvement, int boxTag);
    
    /**
     * @brief play char soud effect.
     * @param action type of character action.
     * @param charNbr character number.
     * @param linkedCharNbr number of the character  linked at the action.
    */
    static void playChar(std::string action, int charNbr, int linkedCharNbr = -1);
    
    /**
     * @brief play char walk soud effect.
     * @param actionTime  char walk atction duration.
     * @param animationTime  char walk animation duration.
     * @param boxStartTag box tag where char start.
     * @param boxFinishTag box tag where char arrive.
    */
    static void playCharWalk(float animationTime, int boxStartTag, int boxFinishTag);
    
    /**
     * @brief play card soud effect.
     * @param saction type of card action.
     * @param cardNbr card number.
    */
    static void playCard(std::string action, int cardNbr);
    
    /**
     * @brief preload all game sound.
    */
    static void preLoad();
    
private:
    
    /**
     * @brief play game soud effect.
     * @param theme sound effect name.
     * @param volume sound volume.
     * @param delay start sound delay
    */
    static void playSound(std::string sound, float volume = 1.0f, float delay = 0.0);
    
    /**
     * @brief star authorization box sound temporisation.
     * @param box pointer to shedules box.
    */
    void boxShedulerStart(cocos2d::Node* box);
    
    /**
     * @brief get and set play sound box authorization.
    */
    void setBoxSoundAuth(bool Auth);
    bool getBoxSoundAuth();
    
    //play sound box authorization.
    bool m_boxSoundAuth;
    
    // ID of the background sound music.
    int m_musicID;
};

//unique instance of MainSouds.
static MainSounds* m_SharedMainSounds = nullptr;

#endif /* MainSounds_hpp */
