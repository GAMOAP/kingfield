//
//  MainSounds.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 08/07/2021.
//

#include "MainSounds.hpp"

#include "audio/include/AudioEngine.h"

#include "MainObject.hpp"
#include "MainStuff.hpp"

USING_NS_CC;

MainSounds* MainSounds::getInstance()
{
    if (!m_SharedMainSounds)
    {
        m_SharedMainSounds = new (std::nothrow) MainSounds;
        m_SharedMainSounds->init();
    }
    
    return m_SharedMainSounds;
}

bool MainSounds::init()
{
    m_SharedMainSounds->setBoxSoundAuth(true);
    return true;
}

void MainSounds::playMusic(std::string theme)
{
    //PLAY GAME MUSIC
}

void MainSounds::playSound(std::string sound,float volume)
{
    // format sound file name
    std::string soundStr = "res/sounds/" + sound + ".mp3";
    
    // play sound (file name, repeat, volume)
    cocos2d::experimental::AudioEngine::play2d(soundStr, false, volume);
}

void MainSounds::playBox(std::string mouvement, int boxTag)
{
    //get box object
    auto box = MainObject::getBoxByTag(boxTag);
    if(box)
    {
        //play box sound if box si in playing surface, is not backgroud type and if the previous sound started from the time indicated
        if(m_SharedMainSounds->getBoxSoundAuth() == true &&
           box->getType() != "background" &&
           (boxTag >= 11 && boxTag <= 65))
        {
            //play the box sound.
            std::string boxBreed = box->getBreed();
            playSound("box_" + boxBreed + "_" + mouvement, 0.2);
            
            
            //set a delay to start the future sound.
            float delayTime = 0.1;
            Director::getInstance()->getScheduler()->schedule([=](float){
                m_SharedMainSounds->setBoxSoundAuth(true);
            }, box, delayTime, 0, 0, false, "BOX_MOVE_DELAY");
            
            m_SharedMainSounds->setBoxSoundAuth(false);
        }
    }
}
void MainSounds::playChar(std::string action, int charNbr)
{
    //get char breed
    std::string breed = MainStuff::getStuffByName(charNbr, 8)[0];
    
    
    printf("breed = %s\n", breed.c_str());
    
    if(action == "select")
    {
        playSound("select_char", 0.9);
        playSound("select_" + breed, 0.3);
    }
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
        "box_sun_remove", "box_night_remove", "box_time_remove",
        
        //CARD----------------------------
        "card_flip",
        //CHARACTER-----------------------
         //select
        "select_main"
        "select_sun", "select_night", "select_time",
         //weapon
         /*
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

// Play sound box authorization (set & get)
void MainSounds::setBoxSoundAuth(bool Auth)
{
    m_SharedMainSounds->m_boxSoundAuth = Auth;
}
bool MainSounds::getBoxSoundAuth()
{
    return m_boxSoundAuth;
}

