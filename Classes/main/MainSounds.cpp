//
//  MainSounds.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 08/07/2021.
//

#include "MainSounds.hpp"

#include "audio/include/AudioEngine.h"

USING_NS_CC;

void MainSounds::playMusic(std::string theme)
{
    //PLAY GAME MUSIC
}

void MainSounds::playSound(std::string sound)
{
    // format sound file name
    std::string soundStr = "res/sounds/" + sound + ".mp3";
    
    // play sound (file name, repeat, volume)
    cocos2d::experimental::AudioEngine::play2d(soundStr, false, 1.0f);
}

void MainSounds::preLoad()
{
    // create sounds list name
    std::vector<std::string> soundsList = {
        //SOUNDLIST...
        
        //GAME----------------------------
        "tittle", "fight", "win", "lose",
        
        
        
        //BOX-----------------------------
            //add
        "box_sun_add", "box_night_add", "box_time_add",
            //remove
        "box_sun_remove", "box_night_remove", "box_time_remove"
        /*
        //CARD----------------------------
        
        //CHARACTER-----------------------
            //select
        "select_sun_sun", "select_sun_night", "select_sun_time",
        "select_night_sun", "select_night_night", "select_night_time",
        "select_time_sun", "select_time_night", "select_time_time",
        
        //weapon
            //sun (spear)
        "weapon_sun_1", "weapon_sun_2", "weapon_sun_3"
            //night (sword)
        "weapon_night_1", "weapon_night_2", "weapon_night_3"
            //time (bow)
        "weapon_time_1", "weapon_time_2", "weapon_time_3"
         
         */
    };
    
    // load all game sounds
    std::vector<std::string>::iterator soundIt;
    for(soundIt = soundsList.begin(); soundIt != soundsList.end(); soundIt++)
    {
        std::string sound = "res/sounds/" + *soundIt + ".mp3";
        cocos2d::experimental::AudioEngine::preload(sound);
    }
}

