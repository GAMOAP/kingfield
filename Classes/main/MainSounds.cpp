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

void MainSounds::playGame(std::string sound)
{
    playSound("game_" + sound);
}

void MainSounds::playBox(std::string mouvement, int boxTag)
{
    //get box object
    auto box = MainObject::getBoxByTag(boxTag);
    if(box)
    {
        if(mouvement == "upDown")
        {
            playSound("box_upDown", 1);
        }
        
        //play box sound if box si in playing surface, is not backgroud type and if the previous sound started from the time indicated
        else if(m_SharedMainSounds->getBoxSoundAuth() == true &&
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
    std::string job = MainStuff::getStuffByName(charNbr, 7)[0];
    
    //select sound
    if(action == "select")
    {
        playSound("char_select_main", 0.9);
        playSound("char_select_" + breed, 0.3);
    }
    
    //stuff sound
    if(action.substr(0, 5) == "stuff")
    {
        std::string type = action.substr(6, action.size());
        if(type == "move" || type == "helmet" || type == "armor")
        {
            playSound("char_stuff_clothes", 0.8);
        }
        if(type == "spell" || type == "weapon")
        {
            playSound("char_stuff_weapon", 0.8);
        }
        if(type == "item" || type == "object")
        {
            playSound("char_stuff_object", 0.8);
        }
        if(type == "breed")
        {
            playSound("char_select_" + breed, 0.3);
        }
        if(type == "job")
        {
            playSound("char_job_" + job, 0.8);
        }
    }
    
    //action sound
    if(action.substr(0, 6) == "action")
    {
        
    }
}
void MainSounds::playCharWalk(float animationTime, int startBoxTag, int finishBoxTag)
{
    printf("[MS] MainSounds::playCharWalk\n");
    
    auto character = MainObject::getCharByTag(startBoxTag);
    if(character)
    {
        
        float stepTime = 0.15;
        int stepNbr = animationTime / stepTime - 1;
        
        int boxTag = startBoxTag;
        
        std::string boxBreed  = MainObject::getBoxByTag(boxTag)->getBreed();
        
        Director::getInstance()->getScheduler()->schedule([=](float){
            
            playSound("char_step_" + boxBreed, 0.3 + random(0, 5)/10);
            
        }, character, stepTime, stepNbr , 0, false, "CHAR_WALK");
    }
    
    //Director::getInstance()->getScheduler()->schedule(const ccSchedulerFunc &callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string &key)
    
}

void MainSounds::playCard(std::string action, int cardNbr)
{
    if(action == "flip")
    {
        playSound("card_flip", 0.5);
    }
    if(action == "select")
    {
        playSound("card_select");
    }
}

void MainSounds::preLoad()
{
    // create sounds list name
    std::vector<std::string> soundsList = {
        //SOUNDLIST...
        
        //GAME----------------------------
        "game_tittle", "game_fight", "game_win", "game_lose", "game_button_social", "game_button_library",
        
        //BOX-----------------------------
            //add
        "box_sun_add", "box_night_add", "box_time_add",
            //remove
        "box_sun_remove", "box_night_remove", "box_time_remove",
        
        //CARD----------------------------
        "card_flip", "card_select",
        //CHARACTER-----------------------
            //select
        "char_select_main", "char_select_sun", "char_select_night", "char_select_time",
            //stuff
        "char_stuff_clothes", "char_stuff_weapon", "char_stuff_object",
                //stuff job
        "char_job_sun", "char_job_night", "char_job_time",
        
       //CHARACTER STEP
        "char_step_sun", "char_step_night", "char_step_time"
    };
    
    // load all game sounds
    std::vector<std::string>::iterator soundIt;
    for(soundIt = soundsList.begin(); soundIt != soundsList.end(); soundIt++)
    {
        std::string sound = "res/sounds/" + *soundIt + ".mp3";
        cocos2d::experimental::AudioEngine::preload(sound);
    }
}

//----------------------------PRIVATE-------------------------------------

void MainSounds::playSound(std::string sound,float volume)
{
    // format sound file name
    std::string soundStr = "res/sounds/" + sound + ".mp3";
    
    // play sound (file name, repeat, volume)
    cocos2d::experimental::AudioEngine::play2d(soundStr, false, volume);
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

