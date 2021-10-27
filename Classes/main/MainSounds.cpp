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
        std::string boxBreed = box->getBreed();
        
        if(boxTag >= 11 && boxTag <= 65)
        {
            if(mouvement == "upDown" &&
               boxTag >= 21 &&
               box->getType() == "field")
            {
                playSound("box_upDown", 1);
            }
            else if(mouvement == "add" &&
                    m_SharedMainSounds->getBoxSoundAuth() == true&&
                    box->getType() != "" &&
                    box->getType() != "background")
            {
                playSound("box_" + boxBreed + "_add", 0.2);
                m_SharedMainSounds->boxShedulerStart(box);
            }
            else if(mouvement == "remove" &&
                    m_SharedMainSounds->getBoxSoundAuth() == true &&
                    box->getLastType() != "" &&
                    box->getLastType() != "background"
                    )
            {
                playSound("box_" + boxBreed + "_remove", 0.2);
                m_SharedMainSounds->boxShedulerStart(box);
            }
        }
    }
}

void MainSounds::playChar(std::string action, int charNbr, int linkedCharNbr)
{
    printf("[MS] MainSounds::playChar(%s, %i)\n", action.c_str(), charNbr);
    
    
    //get char breed
    std::string breed = MainStuff::getStuffByName(charNbr, 8)[0];
    std::string job = MainStuff::getStuffByName(charNbr, 7)[0];
    std::string weapon = MainStuff::getStuffByName(charNbr, 2)[1];
    
    std::string weaponLinked = weapon;
    if(linkedCharNbr >= 0)
    {
        weaponLinked = MainStuff::getStuffByName(linkedCharNbr, 2)[1];
    }
    
    //select sound
    if(action == "select")
    {
        printf("[MS][SLT] char_select_%s\n", breed.c_str());
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
    
    //expression sound
    if(action.substr(0, 10) == "expression")
    {
        printf("[MS] expression = %s\n", action.c_str());
        
        std::string type = action.substr(11, action.size());
        if(type == "tired" ||
           type == "sleep" ||
           type == "sick" ||
           type == "dizzy"
           )
        {
            playSound("char_" + type, 0.1);
        }
    }
    
    //animation sound
    if(action.substr(0, 9) == "animation")
    {
        
        
        std::string type = action.substr(10, action.size());
        if(type == "attack")
        {
            playSound("char_attack_" + weapon, 0.8, 0.8);
        }
        if(type == "pain")
        {
            playSound("char_hit_" + weaponLinked, 0.8);
        }
        if(type == "block")
        {
            playSound("char_hit_block", 0.8);
        }
    }
}
void MainSounds::playCharWalk(float animationTime, int startBoxTag, int finishBoxTag)
{
    auto character = MainObject::getCharByTag(startBoxTag);
    if(character)
    {
        
        float stepTime = 0.15;
        int stepNbr = animationTime / stepTime - 1;
        
        int boxTag = startBoxTag;
        
        std::string boxBreed  = MainObject::getBoxByTag(boxTag)->getBreed();
        
        playSound("walk_effect", 0.2);
        
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
        
            //step
        "char_step_sun", "char_step_night", "char_step_time",
                //step effect
        "walk_effect_0", "walk_effect_1", "walk_effect_2",
            //action
        "char_tired", "char_fail", "char_fall",
            //attack
        "char_attack_sun", "char_attack_night", "char_attack_time",
            //hit
        "char_hit_sun", "char_hit_night", "char_hit_time", "char_hit_block"
        
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

void MainSounds::playSound(std::string sound,float volume, float delay)
{
    //
    printf("[MS] ==> MainSounds::playSound(%s, %f)\n", sound.c_str(), volume);
    
    
    // format sound file name
    std::string soundStr = "res/sounds/" + sound + ".mp3";
    
    // play sound (file name, repeat, volume)
    if(delay <= 0.0)
    {
        cocos2d::experimental::AudioEngine::play2d(soundStr, false, volume);
    }
    else
    {
        // delay to start sound
        Director::getInstance()->getScheduler()->schedule([=](float){
            cocos2d::experimental::AudioEngine::play2d(soundStr, false, volume);
        }, m_SharedMainSounds, delay, 0, 0, false, "SOUND_DELAY");
    }
    
}

void MainSounds::boxShedulerStart(cocos2d::Node* box)
{
    m_SharedMainSounds->setBoxSoundAuth(false);
    
    //set a delay to start the future sound.
    float delayTime = 0.1;
    Director::getInstance()->getScheduler()->schedule([=](float){
        m_SharedMainSounds->setBoxSoundAuth(true);
    }, box, delayTime, 0, 0, false, "BOX_MOVE_DELAY");
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

