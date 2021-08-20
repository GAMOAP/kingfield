//
//  MainSounds.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 08/07/2021.
//

#ifndef MainSounds_hpp
#define MainSounds_hpp

#include "cocos2d.h"

class MainSounds
{
public:
    
    /**
     * @brief play game music
     * @param theme music theme name.
    */
    static void playMusic(std::string theme);
    
    /**
     * @brief play game soud effect.
     * @param theme sound effect name.
    */
    static void playSound(std::string sound);
    
    /**
     * @brief preload all game sound.
    */
    static void preLoad();
    
private:
    
    
};

#endif /* MainSounds_hpp */
