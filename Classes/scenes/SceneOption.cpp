//
//  SceneOption.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 03/10/2019.
//

#include "SceneOption.hpp"

#include "KFSprite.hpp"

#include "MainObject.hpp"
#include "MainGrid.hpp"

#include "GameDirector.hpp"
#include "GameCharacters.hpp"
#include "GameCards.hpp"

#include "Character.hpp"
#include "CharacterDisplay.hpp"
#include "CharacterUI.hpp"

#include "Card.hpp"
#include "CardDisplay.hpp"

#include <iostream>

USING_NS_CC;

SceneOption* SceneOption::setScene(std::string oldScene, int tag)
{
    if (!m_SharedSceneOption)
    {
        m_SharedSceneOption = new (std::nothrow) SceneOption;
        m_SharedSceneOption->init();
    }
    
    m_SharedSceneOption->m_oldScene = oldScene;
    m_SharedSceneOption->m_focusedNode = MainObject::getNodeByTag(tag, true);
    return m_SharedSceneOption;
}

bool SceneOption::init()
{
    setName("option");
    return true;
}

void SceneOption::addToStage()
{
    initFocus();
    addObjects();
}

void SceneOption::removeToStage()
{
    resetFocus();
    GameDirector::setScene(m_oldScene);
}

bool SceneOption::touchBox(int tag)
{
    m_removeAuth = true;
    
    return true;
}

bool SceneOption::unTouchBox(int tag)
{
    if(m_removeAuth && !m_removing)
    {
        removeObjects();
    }
    
    return true;
}

void SceneOption::addObjects()
{
    m_removeAuth = true;
    m_removing = false;
    
    //Init removing time
    m_time = utils::getTimeInMilliseconds();
    
    auto fadeIn = FadeIn::create(m_fadeTime);
    auto callFunc= CallFunc::create([=]()
    {
        auto charUI = m_cloneNode->getChildByName("charUI");
        if(charUI)
            charUI->setVisible(true);
        
        m_removeAuth = false;
    });
    m_maskSeq = Sequence::create(fadeIn, callFunc, NULL);
    m_mask->runAction(m_maskSeq);
    
    Vec3 nodeCenterBox = MainGrid::getPositionXYZ(m_cloneNodeLC.x, m_cloneNodeLC.y);
    auto moveCenter = MoveTo::create(m_fadeTime, Vec2(nodeCenterBox.x, nodeCenterBox.y));
    auto scale = ScaleTo::create(m_fadeTime, m_cloneNodeScale);
    auto nodeFadeIn = FadeIn::create(m_fadeTime);
    auto spawnMove = Spawn::createWithTwoActions(moveCenter, scale);
    auto spawnNode = Spawn::createWithTwoActions(spawnMove, nodeFadeIn);
    
    m_nodeSeq = Sequence::create(spawnNode, NULL);
    m_cloneNode->runAction(m_nodeSeq);
}

void SceneOption::removeObjects()
{
    m_mask->getActionManager()->removeAction(m_maskSeq);
    m_cloneNode->getActionManager()->removeAction(m_nodeSeq);
    
    //Removing time.
    float t = utils::getTimeInMilliseconds() - m_time;
    float fadeTime = t / 1000;
    if(fadeTime >= m_fadeTime)
        fadeTime = m_fadeTime;
    
    auto charUI = m_cloneNode->getChildByName("charUI");
    if(charUI)
        charUI->setVisible(false);
    
    m_removing = true;
    
    auto maskFadeOut = FadeOut::create(fadeTime);
    auto callFunc= CallFunc::create([=]()
    {
        m_mask->removeFromParent();
        m_cloneNode->removeFromParent();
        removeToStage();
        
    });
    m_maskSeq = Sequence::create(maskFadeOut, callFunc, NULL);
    m_mask->runAction(m_maskSeq);
     
    Vec3 nodeCenterBox = MainGrid::getPositionXYZ(m_cloneNodeLC.x, m_cloneNodeLC.y);
    auto movePlace = MoveTo::create(fadeTime, m_focusedNode->getPosition());
    auto scale = ScaleTo::create(fadeTime, 1);
    auto nodeFadeOut = FadeOut::create(m_fadeTime);
    auto spawnMove = Spawn::createWithTwoActions(movePlace, scale);
    auto spawnNode = Spawn::createWithTwoActions(spawnMove, nodeFadeOut);
     
    m_nodeSeq = Sequence::create(spawnNode, NULL);
    m_cloneNode->runAction(m_nodeSeq);
}

void SceneOption::initFocus()
{
    Vec3 maskCenterBox = MainGrid::getPositionXYZ(2, 2);
    
    m_mask = Sprite::create(KFSprite::getFile("screen_help_mask"));
    m_mask->setAnchorPoint(Vec2( 0.5, 0.5));
    m_mask->setPosition(Vec2(maskCenterBox.x, maskCenterBox.y));
    m_mask->setOpacity(0);
    
    auto character = dynamic_cast<Character*>(m_focusedNode);
    auto card = dynamic_cast<Card*>(m_focusedNode);
    if(character)
    {
        int charNbr = character->getNumber();
        m_cloneNode = CharacterDisplay::create(charNbr);
        m_cloneNodeLC = Vec2(1, 2);
        m_cloneNodeScale = 3.4;
        
        auto charUI = CharacterUI::getOptionCharacterUI(charNbr);
        charUI->setVisible(false);
        charUI->setScale(0.77);
        m_cloneNode->addChild(charUI,2);
    }
    if(card)
    {
        m_cloneNode = CardDisplay::create(card->getType(), card->getBreed(), card->getObject(), card->getBoard());
        m_cloneNodeLC = Vec2(2, 2);
        m_cloneNodeScale = 2.9;
    }
    
    m_cloneNode->setPosition(m_focusedNode->getPosition());
    m_cloneNode->setCascadeOpacityEnabled(true);
    m_cloneNode->setOpacity(0);
    
    MainObject::getInfoLayer()->addChild(m_mask,0);
    MainObject::getInfoLayer()->addChild(m_cloneNode,1);
}
void SceneOption::resetFocus()
{
    auto character = dynamic_cast<Character*>(m_focusedNode);
    auto card = dynamic_cast<Card*>(m_focusedNode);
    if(character)
    {
        
    }
    if(card)
    {
        
    }
}



/*
//Removing time.
float t = utils::getTimeInMilliseconds() - m_time;
float time = t / 1000;
if(time >= m_fadeTime)
    time = m_fadeTime + 0.1;

//auto delay = DelayTime::create(0.1);

printf("start");
auto scale = ScaleTo::create(time, 1);
auto movePlace = MoveTo::create(time, Vec2(m_nodePosition.x, m_nodePosition.y));
auto spawn = Spawn::createWithTwoActions(scale, movePlace);
auto callEndFunc= CallFunc::create([=]()
{
    printf("end\n");
    m_focusedNode->setLocalZOrder(m_nodeIndex);
    m_mask->removeFromParent();
    
    removeToStage();
});
auto seq = Sequence::create(spawn, callEndFunc, NULL);
m_focusedNode->runAction(seq);

printf("test");

//Remove Mask
m_mask->getActionManager()->removeAction(m_maskSeq);

auto fadeOut = FadeOut::create(time);
m_maskSeq = Sequence::create(fadeOut, NULL);
m_mask->runAction(m_maskSeq);*/



/*
//Init removing time
m_time = utils::getTimeInMilliseconds();
m_focusNodeScale = 3.1 - (m_focusedNode->getDisplayNode()->getScale() - 1);

//Init node origin position
m_nodePosition = m_focusedNode->getPosition();
m_nodeIndex = m_focusedNode->getLocalZOrder();

//Add Focused Node
m_focusedNode->setLocalZOrder(32);

Vec3 nodeCenterBox = MainGrid::getPositionXYZ(1, 2);

auto moveCenter = MoveTo::create(m_fadeTime, Vec2(nodeCenterBox.x, nodeCenterBox.y));
auto scale = ScaleTo::create(m_fadeTime, m_focusNodeScale);
auto spawn = Spawn::createWithTwoActions(moveCenter, scale);

m_nodeSeq = Sequence::create(spawn, NULL);
m_focusedNode->runAction(m_nodeSeq);

MainObject::getMainLayer()->addChild(m_mask, 31);

auto fadeIn = FadeIn::create(m_fadeTime);
auto callFunc= CallFunc::create([=]()
{
    m_removeAuth = false;
});
m_maskSeq = Sequence::create(fadeIn, callFunc, NULL);
m_mask->runAction(m_maskSeq);
 */
