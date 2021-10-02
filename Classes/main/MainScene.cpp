//
//  MainScene.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 28/04/2019.
//

#include "MainScene.hpp"

#include "BoxMask.hpp"

#include <iostream>

USING_NS_CC;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    //MainUser::reset();
    
    auto director = Director::getInstance();
    director->setClearColor(Color4F(255, 255, 255, 255));
    
    //Create game Grid
    MainGrid::getInstance();
    
    //Create MainLayer
    auto mainLayer = Node::create();
    addChild(mainLayer, 0);
    
    //Create InfoLayer
    auto infoLayer = Node::create();
    addChild(infoLayer, 1);
    
    //Create game Object
    auto mainObject = MainObject::getInstance();
    mainObject->setMainLayer(mainLayer);
    mainObject->setInfoLayer(infoLayer);
    
    //Create game Stuff
    MainStuff::getInstance();
    
    //Create game Sounds
    MainSounds::getInstance();
    
    //Init Game Director
    GameDirector::getInstance();
    
    //Init GameMultiplayer
    MainMultiPlayer::getInstance();
    
    //Create Mask
    for(int l = m_mainGrid->getOriginLine(); l < m_mainGrid->getLineNumber() + 1; l++)
    {
        BoxMask::createMask(l);
    }
    
    //Add Touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(MainScene::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    //add Node Listener.
    auto IsAllEvent = EventListenerCustom::create("ALL_NODE_IS_IN", [](EventCustom* event)
    {
        GameDirector::allNodeIsIn();
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(IsAllEvent, this);
    
    //Start
    GameDirector::setScene("intro");
    return true;
}

//touch
bool MainScene::onTouchBegan(Touch* touch, Event* event)
{
    const int touchBoxTag = m_mainGrid->getTagByLocation(touch->getLocation());
    
    m_touchBoxTag = touchBoxTag;
    GameDirector::touchBox(m_touchBoxTag);
        
    cancelTouchLong();
    
    auto delay = DelayTime::create(0.5);
    auto callFunc = CallFunc::create([=]()
    {
        onTouchLong(touchBoxTag);
    });
    m_longTouch = Sequence::create(delay, callFunc, NULL);
    this->runAction(m_longTouch);
    
    return true;
}

//unTouch
void MainScene::onTouchEnded(Touch* touch, Event* event)
{
    cancelTouchLong();
    const int touchboxTagEnded = MainGrid::getTagByLocation(touch->getLocation());
    if(m_touchBoxTag == touchboxTagEnded)
    {
        GameDirector::unTouchBox(touchboxTagEnded);
    }
}

//moveTouch
void MainScene::onTouchMoved(Touch* touch, Event* event)
{
    const int touchboxTagMoved = MainGrid::getTagByLocation(touch->getLocation());
    if(m_touchBoxTag != touchboxTagMoved)
    {
        cancelTouchLong();
        GameDirector::cancelTouchBox(m_touchBoxTag);
    }
}

//cancelTouch
void MainScene::onTouchCancelled(Touch* touch, Event* event)
{
    cancelTouchLong();
    GameDirector::cancelTouchBox(m_touchBoxTag);
}

//longTouch
void MainScene::onTouchLong(int boxTag)
{
    if(boxTag == m_touchBoxTag)
    {
        GameDirector::longTouchBox(m_touchBoxTag);
    }
}
void MainScene::cancelTouchLong()
{
    this->getActionManager()->removeAction(m_longTouch);
    m_longTouch = nullptr;
}
