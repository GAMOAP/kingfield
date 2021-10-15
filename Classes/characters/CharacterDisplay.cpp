//
//  CharacterDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//
#include "Constants.h"

#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainStuff.hpp"
#include "MainSounds.hpp"

#include "CharacterDisplay.hpp"

#include <iostream>

USING_NS_CC;

CharacterDisplay* CharacterDisplay::create(int number)
{
    CharacterDisplay* ret = new (std::nothrow) CharacterDisplay();
    if(ret && ret->init (number))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
        return ret;
}

bool CharacterDisplay::init(int number)
{
    setName("displayNode");
    
    //init number, stuff and dragonBones factory.
    m_number = number;
    m_alive = true;
    setStuffList();
    createFactory();
    
    if(number == 2 || number == 7)
    {
        setFlag();
    }
    
    this->setCascadeColorEnabled(true);
    
    return true;
}

bool CharacterDisplay::createFactory()
{
    //test dragonBones Factory files.
    std::string skeFile = "res/dragonBones/character_animation_ske.json";
    std::string texFile = "res/dragonBones/character_animation_tex.json";
    auto files = FileUtils::getInstance();
    
    bool isSkeExist = files->isFileExist(skeFile);
    bool isTexExist = files->isFileExist(texFile);
    
    if (!isSkeExist || !isTexExist)
    {
        return false;
    }
    
    //create dragonBones factory.
    m_factory = dragonBones::CCFactory::getFactory();
    m_factory->loadDragonBonesData(skeFile,"name");
    m_factory->loadTextureAtlasData(texFile,"name");
    m_armatureDisplay = m_factory->buildArmatureDisplay("Armature");
    
    //animation start delay.
    float randDelay = rand()%100;
    auto delay = DelayTime::create(randDelay/100);
    auto callFunc = CallFunc::create([this](){ this->playAnimation();});
    auto seq = Sequence::create(delay, callFunc, NULL);
    this->runAction(seq);
    
    //animation init.
    setAnimation("stand");
    this->setScale(1.2);
    this->setColor(m_colorUnselect);
    
    //animation event init.
    m_armatureDisplay->getEventDispatcher()->setEnabled(true);
    m_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, [=](cocos2d::Event* event)
    {
        animationEnd(event);
    });
    
    //position init.
    m_armatureDisplay->setPosition(0.0f, 0.0f);
    this->addChild(m_armatureDisplay, 1);
    
    //stuff display init.
    setStuffDisplay();
    
    return true;
}

//----------------STUFF-------------------
void CharacterDisplay::setStuffList()
{
    auto mainStuff = MainStuff::getInstance();
    for(int i = 0; i < CARD_TYPE.size(); i++)
    {
        m_stuffList[CARD_TYPE[i]] = mainStuff->getStuffByName(m_number, i);
    }
}
void CharacterDisplay::setStuffDisplay()
{
    m_outlineSpriteList.clear();
    
    for(int i = 0; i < CHAR_SLOTS.size();  i++)
    {
        std::string slotName = CHAR_SLOTS[i];
        std::string slotFileDisplay = getSlotFileDisplay(slotName);
        std::string textureName, outlineName;
        if(slotFileDisplay != "")
        {
            textureName = "char/texture/" + slotFileDisplay + ".png";
            outlineName = "char/outline/" + slotFileDisplay + ".png";
        }
        else
        {
            textureName = outlineName = "screen/void.png";
        }
        
        auto slot = m_armatureDisplay->getArmature()->getSlot("character_" + slotName);
        auto image = cocos2d::Sprite::createWithSpriteFrameName(textureName);
        image->cocos2d::Node::setAnchorPoint(Vec2(64,64));
        slot->setDisplay(image, dragonBones::DisplayType::Image);
        
        auto slotOutline = m_armatureDisplay->getArmature()->getSlot("character_outline_" + slotName);
        auto imageOutline = cocos2d::Sprite::createWithSpriteFrameName(outlineName);
        imageOutline->cocos2d::Node::setAnchorPoint(Vec2(64,64));
        imageOutline->setName(slotName);
        slotOutline->setDisplay(imageOutline, dragonBones::DisplayType::Image);
        
        m_outlineSpriteList.push_back(imageOutline);
    }
    
    setOutline();
}
std::string CharacterDisplay::getSlotFileDisplay(std::string& slotName)
{
    std::string file = "";
    
    if(slotName == "head" || slotName == "face" || slotName == "hand")
    {
        file = m_stuffList["breed"][0] +"/"+ m_stuffList["job"][0] +"_"+ slotName;
    }
    else if (slotName == "helmet")
    {
        file = m_stuffList["helmet"][0] +"/"+ m_stuffList["helmet"][1] +"_"+ slotName;
    }
    else if (slotName == "armor" || slotName == "arm")
    {
        file = m_stuffList["armor"][0] +"/"+ m_stuffList["armor"][1] +"_"+ slotName;
    }
    else if (slotName == "item_arm" || slotName == "item_armor" || slotName == "item_hand")
    {
        std::string itemType = m_stuffList["item"][1];
        if((itemType == "sun" && slotName == "item_armor") ||
           (itemType == "night" && slotName == "item_arm") ||
           (itemType == "time" && slotName == "item_hand"))
            file = m_stuffList["item"][0] +"/"+ m_stuffList["item"][1] +"_item";
        else
            file = "";
    }
    else if (slotName == "frontLeg" || slotName == "backLeg")
    {
        file = m_stuffList["move"][0] +"/"+ m_stuffList["move"][1] +"_"+ slotName;
    }
    else if (slotName == "weapon")
    {
        file = m_stuffList["weapon"][0] +"/"+ m_stuffList["weapon"][1] +"_"+ slotName;
    }
    else
    {
        // NOT VALID SLOT;
    }
    
    return file;
}
//---------------OUTLINE-------------------
bool CharacterDisplay::setOutline(OutlineColor outlineColor, float lineSize)
{
    Color3B outlineColor3B = Color3B::WHITE;
    switch (outlineColor) {
        case white:
            outlineColor3B = Color3B::WHITE;
            break;
        case black:
            outlineColor3B = Color3B::BLACK;
            break;
        case blue:
            outlineColor3B = Color3B::BLUE;
            break;
        case red:
            outlineColor3B = Color3B::RED;
            break;
            
        default:
            break;
    }
    
    for(int o = 0; o < m_outlineSpriteList.size();  o++)
    {
        Sprite* outlineSprite = m_outlineSpriteList[o];
        outlineSprite->setColor(outlineColor3B);
    }
    
    return true;
}

//----------------ACTION-------------------
//stuff.
bool CharacterDisplay::setStuff(std::string type)
{
    setAnimation("equip", 1);
    
    MainSounds::playChar("stuff_" + type, m_number);
    
    auto delay = DelayTime::create(0.5);
    auto func = CallFunc::create([this](){
        setStuffList();
        setStuffDisplay();
    });
    auto seq = Sequence::create(delay, func, NULL);
    this->runAction(seq);
    
    
    return true;
}
//flag.
bool CharacterDisplay::setFlag()
{
    std::string flagName = MainStuff::getTeamKarma(m_team);
    
    if(m_flag)
    {
        m_flag->removeFromParent();
        m_flag = nullptr;
    }
    
    m_flag = Sprite::createWithSpriteFrameName("UI/char/flag_" + flagName + "_" + std::to_string(0) + ".png");
    m_flag->setAnchorPoint(Vec2(1,0));
    
    if(m_number == 7)
    {
        m_flag->setScaleX(-1);
        m_flagPos.x -= m_flag->getContentSize().width/2;
    }
    m_flag->setPosition(m_flagPos);
    this->addChild(m_flag, 0);
    
    cocos2d::Vector<cocos2d::SpriteFrame*> flagFrame;
    flagFrame.reserve(7);
    auto spriteCache = SpriteFrameCache::getInstance();
    for(int f = 0; f < 7; f++)
    {
        std::string animName = "UI/char/flag_" + flagName + "_" + std::to_string(f) + ".png";
        flagFrame.pushBack(spriteCache->getSpriteFrameByName(animName.c_str()));
    }
    Animation* flagAnimation = Animation::createWithSpriteFrames(flagFrame, 0.1f);
    Animate* flagAnimate = Animate::create(flagAnimation);
    m_flag->runAction(RepeatForever::create(flagAnimate));
    
    return true;
}
//select.
void CharacterDisplay::setSelect()
{
    setAnimation("select");
    if(!m_selected)
    {
        setAnimation("equip", 1);
        MainSounds::playChar("select", m_number);
    }
    
    if(m_number == 2 || m_number == 7)
    {
        setScale(m_scaleArray[3]);
    }
    else
    {
        setScale(m_scaleArray[1]);
    }
    
    setColor(m_colorSelect);
    
    setOutline(red, 1);
    
    
    m_selected = true;
}
void CharacterDisplay::setUnselect()
{
    setAnimation("stand");
    
    if(m_number == 2 || m_number == 7)
    {
        setScale(m_scaleArray[2]);
    }
    else
    {
        setScale(m_scaleArray[0]);
    }
    
    setColor(m_colorUnselect);
    
    setOutline(black, 0.5);
    
    m_selected = false;
}

//----------------ANIMATION-------------------
bool CharacterDisplay::setAnimation(std::string animation, int nbrLoop, bool playLastAnimation)
{
    m_animationLoopNumber = nbrLoop;
    m_animationVector[0] = animation;
    m_playLastAnimation = playLastAnimation;
    playAnimation();
    
    return true;
}

bool CharacterDisplay::setState(std::string state)
{
    m_animationVector[1] = state;
    
    return true;
}

bool CharacterDisplay::playAnimation()
{
    dragonBones::Animation* animation = m_armatureDisplay->getAnimation();
    
    m_lastAnimationName = animation->getLastAnimationName();
    std::string animationName = m_animationVector[0] + "_" + m_animationVector[1];
    
    //check if animation exist in animation list and replace if not.
    std::vector<std::string> animationsNames = animation->getAnimationNames();
    std::vector<std::string>::iterator anIt = std::find(animationsNames.begin(), animationsNames.end(), animationName);
    if(anIt == animationsNames.end())
    {
        animationName = m_animationVector[0] + "_ok";
    }
    
    if(animationName != m_lastAnimationName && m_alive)
    {
        if(TEST_CHAR_ANIM_ON)printf("ANIMATION_START :: char#%i, name = %s\n", m_number, animationName.c_str());
        
        if(m_animationVector[0] == "death")
            m_alive = false;
        
        animation->fadeIn(animationName, 0.2, m_animationLoopNumber);
        setExpression(animationName);
        m_animationLoopNumber = 0;
    }
        
    return true;
}

bool CharacterDisplay::animationEnd(cocos2d::Event* event)
{
    dragonBones::Animation* animation = m_armatureDisplay->getAnimation();
    
    //get animation an supprim state part.
    std::string lastAnimationName = animation->getLastAnimationName();
    std::string name = lastAnimationName.substr(0, lastAnimationName.find("_"));
    
    if(TEST_CHAR_ANIM_ON)printf("ANIMATION_END :: char#%i, endName = %s", m_number, lastAnimationName.c_str());
    
    //dispatch end of animation.
    _eventDispatcher->dispatchCustomEvent("CHAR_" + std::to_string(m_number) + "_ANIM_" + name + "_END");
    _eventDispatcher->removeCustomEventListeners("CHAR_" + std::to_string(m_number) + "_ANIM_" + name + "_END");
    
    //run the animation before this animation end.
    if(!m_playLastAnimation)
    {
        animation->gotoAndStopByFrame(lastAnimationName, 60);
        setExpression(lastAnimationName);
        m_playLastAnimation = false;
    }
    else if(lastAnimationName != m_lastAnimationName)
    {
        if(TEST_CHAR_ANIM_ON)printf(", startName = %s", m_lastAnimationName.c_str());
        
        animation->fadeIn(m_lastAnimationName, 0.2);
        setExpression(m_lastAnimationName);
    }
    
    if(TEST_CHAR_ANIM_ON)printf(", event = CHAR_%i_ANIM_%s_END\n", m_number, name.c_str());
    
    return true;
}

void CharacterDisplay::stopAnimation()
{
    m_armatureDisplay->getAnimation()->reset();
}

//----------------EXPESSION-------------------
bool CharacterDisplay::setExpression(std::string animationName)
{
    std::string aName = animationName.substr(0, animationName.find("_"));
    std::string aState = animationName.substr(animationName.find("_") + 1, animationName.size());
    
    std::string expressionName = m_stuffList["job"][0];
    
    //expression are classed by inverse priority
    if(animationName == "equip_ok"){expressionName = "happy";}
    
    if(aName == "attack"){expressionName = "angry";}
    if(aName == "block"){expressionName = "happy";}
    
    if(m_animationVector[1] == "sleep"){expressionName = "sleep";}
    if(m_animationVector[1] == "poison"){expressionName = "sick";}
    if(m_animationVector[1] == "tired"){expressionName = "tired";}
    
    if(aName == "fail"){expressionName = "dizzy";}
    if(aName == "pain"){expressionName = "pain";}
    if(aName == "sad"){expressionName = "pain";}
    if(aName == "happy"){expressionName = "pain";}
    if(aName == "dead"){expressionName = "pain";}
    
    std::string expressionFile = "char/texture/" + m_stuffList["breed"][0] +"/"+ expressionName + "_face.png";
    
    auto slot = m_armatureDisplay->getArmature()->getSlot("character_face");
    auto image = cocos2d::Sprite::createWithSpriteFrameName(expressionFile);
    image->cocos2d::Node::setAnchorPoint(Vec2(64,64));
    slot->setDisplay(image, dragonBones::DisplayType::Image);
    
    std::string expressionFileOutline = "char/outline/" + m_stuffList["breed"][0] +"/"+ expressionName + "_face.png";
    auto slotOutline = m_armatureDisplay->getArmature()->getSlot("character_outline_face");
    auto imageOutline = cocos2d::Sprite::createWithSpriteFrameName(expressionFileOutline);
    imageOutline->cocos2d::Node::setAnchorPoint(Vec2(64,64));
    imageOutline->setName("face");
    
    for(int o = 0; o < m_outlineSpriteList.size();  o++)
    {
        Sprite* outlineSprite = m_outlineSpriteList[o];
        if(outlineSprite && outlineSprite->getName() == "face")
        {
            m_outlineSpriteList[o] = imageOutline;
        }
    }
    
    slotOutline->setDisplay(imageOutline, dragonBones::DisplayType::Image);
    
    return true;
}

//-----------------INFO FUNCTION--------------------
void CharacterDisplay::setInfo(std::string infoName, int infoValue)
{
    if(!m_charInfo)
    {
        m_charInfo = CharacterInfo::create();
        if(m_number >= 5){ m_charInfo->setScaleX(-1);}
        this->addChild(m_charInfo, 32);
    }
    m_charInfo->addToInfoList(m_number, infoName, infoValue);
}

//----------------RETURN FUNCTION-------------------
int CharacterDisplay::getOriginTag(){ return m_originTag;}
int CharacterDisplay::getNumber(){ return m_number;}

std::string CharacterDisplay::getAnimationName(){ return m_animationVector[0];}
std::string CharacterDisplay::getStateName(){ return m_animationVector[1];}
