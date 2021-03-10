//
//  CharacterDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//
#include "Constants.h"

#include "KFSprite.hpp"

#include "MainGrid.hpp"
#include "MainObject.hpp"
#include "MainStuff.hpp"

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
    setStuffList();
    createFactory();
    
    if(number == 2 || number == 7)
        setFlag();
    
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
    float randDelay = rand()%10;
    auto delay = DelayTime::create(randDelay);
    auto callFunc = CallFunc::create([this](){ this->playAnimation();;});
    auto seq = Sequence::create(delay, callFunc, NULL);
    this->runAction(seq);
    //animation init.
    setAnimation("stand");
    this->setScale(1.2);
    this->setColor(m_colorUnselect);
    
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
    for(int i = 0; i < c_cardType.size(); i++)
    {
        m_stuffList[c_cardType[i]] = mainStuff->getStuffByName(m_number, i);
    }
}
void CharacterDisplay::setStuffDisplay()
{
    for(int i = 0; i < c_charSlotNames.size();  i++)
    {
        std::string slotName = c_charSlotNames[i];
        std::string imageFile = getSlotFileDisplay(slotName);
        auto slot = m_armatureDisplay->getArmature()->getSlot("character_" + slotName);
        auto image = cocos2d::Sprite::create(imageFile);
        
        image->cocos2d::Node::setAnchorPoint(Vec2(64,64));
        image->setName(slotName);
        
        slot->setDisplay(image, dragonBones::DisplayType::Image);
    }
}
std::string CharacterDisplay::getSlotFileDisplay(std::string& slotName)
{
    std::string file = "";
    std::string slotDisplayFile = "";
    if(slotName == "head" || slotName == "face" || slotName == "hand")
    {
        file = m_stuffList["breed"][0] + m_stuffList["job"][0] +"_"+ slotName;
    }
    else if (slotName == "helmet")
    {
        file = m_stuffList["helmet"][0] + m_stuffList["helmet"][1] +"_"+ slotName;
    }
    else if (slotName == "armor" || slotName == "arm")
    {
        file = m_stuffList["armor"][0] + m_stuffList["armor"][1] +"_"+ slotName;
    }
    else if (slotName == "item_arm" || slotName == "item_armor" || slotName == "item_hand")
    {
        std::string itemType = m_stuffList["item"][1];
        if((itemType == "sun" && slotName == "item_armor") ||
           (itemType == "night" && slotName == "item_arm") ||
           (itemType == "time" && slotName == "item_hand"))
            file = m_stuffList["item"][0] + m_stuffList["item"][1] +"_item";
        else
            file = "screen_void";
    }
    else if (slotName == "frontLeg" || slotName == "backLeg")
    {
        file = m_stuffList["move"][0] + m_stuffList["move"][1] +"_"+ slotName;
    }
    else if (slotName == "weapon")
    {
        file = m_stuffList["weapon"][0] + m_stuffList["weapon"][1] +"_"+ slotName;
    }
    else
    {
        // NOT VALID SLOT;
    }
    slotDisplayFile = KFSprite::getFile(file);
    
    return slotDisplayFile;
}

//----------------ACTION-------------------
//stuff.
bool CharacterDisplay::setStuff()
{
    setAnimation("equip", 1);
    setStuffList();
    setStuffDisplay();
    
    return true;
}
//flag.
bool CharacterDisplay::setFlag()
{
    std::string karma = MainStuff::getTeamKarma(m_team);
    
    if(m_number != 2 && m_number != 7 && karma == m_karmaFlag)
    {
        return false;
    }
    else
    {
        m_karmaFlag = karma;
        
        if(m_flag)
        {
            m_flag->removeFromParent();
            m_flag = nullptr;
        }
        
        m_flag = Sprite::create(KFSprite::getFile("charUI_flag_" + m_karmaFlag + "_" + std::to_string(0)));
        m_flag->setAnchorPoint(Vec2(1,0));
        m_flag->setPosition(m_flagPos);
        this->addChild(m_flag, 0);
        
        cocos2d::Vector<cocos2d::SpriteFrame*> flagFrame;
        flagFrame.reserve(7);
        for(int f = 0; f < 7; f++)
        {
            flagFrame.pushBack(SpriteFrame::create(KFSprite::getFile("charUI_flag_" + m_karmaFlag + "_" + std::to_string(f)), Rect(0,0,88,52)));
        }
        Animation* flagAnimation = Animation::createWithSpriteFrames(flagFrame, 0.1f);
        Animate* flagAnimate = Animate::create(flagAnimation);
        m_flag->runAction(RepeatForever::create(flagAnimate));
        
        return true;
    }
}
//select.
void CharacterDisplay::setSelect()
{
    setAnimation("select");
    if(!m_selected)
        setAnimation("equip", 1);
    
    if(m_number == 2 || m_number == 7)
        this->setScale(m_scaleArray[3]);
    else
        this->setScale(m_scaleArray[1]);
    
    this->setColor(m_colorSelect);
    m_selected = true;
}
void CharacterDisplay::setUnselect()
{
    setAnimation("stand");
    
    if(m_number == 2 || m_number == 7)
        this->setScale(m_scaleArray[2]);
    else
        this->setScale(m_scaleArray[0]);
    
    this->setColor(m_colorUnselect);
    m_selected = false;
}
//action.
void CharacterDisplay::setAction(std::string actionType)
{
    setAnimation(actionType);
}

//----------------ANIMATION-------------------
void CharacterDisplay::setAnimation(std::string animation, int nbrLoop)
{
    m_animationLoopNumber = nbrLoop;
    m_animationVector[0] = animation;
    playAnimation();
}
void CharacterDisplay::setState(std::string state)
{
    m_animationVector[1] = state;
    playAnimation();
}
void CharacterDisplay::playAnimation()
{
    std::string animationName = m_animationVector[0] + "_" + m_animationVector[1];
    dragonBones::Animation* animation = m_armatureDisplay->getAnimation();
    animation->fadeIn(animationName, 0.2);
    
    if(m_animationLoopNumber > 0){
        m_armatureDisplay->getEventDispatcher()->setEnabled(true);
        m_armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::LOOP_COMPLETE, [=](EventCustom* event)
        {
            m_animationLoopNumber -= 1;
            if(m_animationLoopNumber <= 0)
            {
                m_animationVector[0] = m_oldAnimationVector[0];
                m_animationVector[1] = m_oldAnimationVector[1];
                m_animationLoopNumber = 0;
            }
            _eventDispatcher->removeCustomEventListeners(dragonBones::EventObject::LOOP_COMPLETE);
            m_armatureDisplay->getEventDispatcher()->setEnabled(false);
            playAnimation();
        });
    }
    else
    {
        m_oldAnimationVector[0] = m_animationVector[0];
        m_oldAnimationVector[1] = m_animationVector[1];
    }
}
void CharacterDisplay::stopAnimation()
{
    m_armatureDisplay->getAnimation()->reset();
}

//-----------------INFO FUNCTION--------------------
void CharacterDisplay::setInfo(std::string infoName, int infoValue)
{
    for(int t = 0; t < m_typeNameInfoList.size(); t++)
    {
        if(infoName == m_typeNameInfoList[t])
        {
            const std::vector<int> info = {t,infoValue};
            m_infoList.push_back(info);
        }
    }
    displayInfo();
}
void CharacterDisplay::displayInfo()
{
    if(m_infoList.size() > 0 && !m_infoDisplay)
    {
        createInfo();
        this->addChild(m_infoDisplay);
        m_infoDisplay->setPosition(Vec2(0,64));
        
        const float time = 1;
        auto moveUp = MoveBy::create(time, Vec2(0, 44));
        auto endFunc = CallFunc::create([this]()
        {
            m_infoDisplay->removeFromParent();
            m_infoDisplay = nullptr;
            m_infoList.erase(m_infoList.begin());
            displayInfo();
        });
        auto seq = Sequence::create(moveUp, endFunc, NULL);
        m_infoDisplay->runAction(seq);
    }
}
void CharacterDisplay::createInfo()
{
    std::string infoType = m_typeNameInfoList[m_infoList[0][0]];
    int infoValue = m_infoList[0][1];
    
    m_infoDisplay = Node::create();
    
    std::string infoFileName;
    Vec2 infoPos;
    Color3B infoColor;
    
    if(infoType == "crystal")
    {
        infoFileName = KFSprite::getFile("charUI_info_number_" + std::to_string(abs(infoValue)));
        infoPos = {0, 0};
        infoColor = {41, 171, 226};
        std::string signString = "plus";
        if(infoValue < 0)
            signString = "minus";
        auto sign = Sprite::create(KFSprite::getFile("charUI_info_number_" + signString));
        sign->setPositionX(-36);
        sign->setColor(infoColor);
        sign->setAnchorPoint(Vec2(0,0));
        m_infoDisplay->addChild(sign);
    }
    
    auto infoSprite = Sprite::create(infoFileName);
    infoSprite->setAnchorPoint(Vec2(0,0));
    infoSprite->setPosition(infoPos);
    infoSprite->setColor(infoColor);
    m_infoDisplay->addChild(infoSprite);
}

//----------------RETURN FUNCTION-------------------
int CharacterDisplay::getOriginTag(){ return m_originTag;}
int CharacterDisplay::getNumber(){ return m_number;}
