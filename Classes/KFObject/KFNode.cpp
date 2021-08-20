//
//  KFNode.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 04/07/2019.
//
#include "MainGrid.hpp"
#include "MainObject.hpp"

#include "KFNode.hpp"

#include <iostream>

USING_NS_CC;

KFNode* KFNode::getClassObject()
{
    return this;
}

//------------------------------POSITION---------------------------
bool KFNode::setNodePosition()
{
    auto mainGrid = MainGrid::getInstance();
    
    // get Node position and index on screen by its line and collumn
    m_index = mainGrid->getPositionXYZ(m_line, m_collumn).z + getIndexClassName();
    m_position.x = mainGrid->getPositionXYZ(m_line, m_collumn).x;
    m_position.y = mainGrid->getPositionXYZ(m_line, m_collumn).y;
    
    // set Node position atributs
    this->setPosition(m_position);
    this->setLocalZOrder(m_index);
    this->setTag(mainGrid->getTag(Vec2( m_line, m_collumn)));
    this->setMetaName(m_className + std::to_string(_tag));
    
    return true;
}
void KFNode::setFlip()
{
    m_isFlipped = -1;
}

//-------------------------------ADD REMOVE TO STAGE-------------------------------
bool KFNode::addToStage()
{
    //check object name.
    if(m_className == "")
    {
        printf("NO CLASS NAME");
        return false;
    }
    
    //get screen size.
    m_visibleSize = Director::getInstance()->getVisibleSize();
    
    //set position.
    setNodePosition();
    
    if(m_className == "box")
    {
        addShader();
    }
    
    //add to stage.
    auto mainScene = MainObject::getMainLayer();
    mainScene->addChild(this, m_index);
    
    
    
    return true;
}

bool KFNode::removeToStage()
{
    //if object is present in stage get out of screen.
    if(m_isIn)
    {
        remove();
    }
    
    //wait object is out of screen to remove to stage.
    auto boxIsOutEvent = EventListenerCustom::create("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT", [this](EventCustom* event)
    {
        //dispatch that object is removed
        _eventDispatcher->dispatchCustomEvent("NODE_"+ m_className + std::to_string(_tag)+"_REMOVED", &_tag);
        _eventDispatcher->removeCustomEventListeners("NODE_"+ m_className + std::to_string(_tag)+"_REMOVED");
        this->removeFromParent();
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(boxIsOutEvent, this);
        
    return true;
}

//--------------------------------ADD REMOVE--------------------------------
void KFNode::add(float speedFactor, float delayFactor)
{
    m_isOut = false;
    float randomTime = MainGrid::getRandom(this->getTag());
    float delayTime = randomTime * delayFactor;
    auto delay = DelayTime::create(delayTime);
    auto move = MoveTo::create(m_moveTime * speedFactor, Vec2(m_position.x, m_position.y));
    
    auto moveIn = EaseBackOut::create(move->clone());
    auto scale = ScaleTo::create(0.2, 1 * m_isFlipped, 1);
    auto spawn = Spawn::createWithTwoActions(moveIn, scale);
    auto callFunc = CallFunc::create([=]()
    {
        finishAction("add");
        m_isIn = true;
        _eventDispatcher->dispatchCustomEvent("NODE_"+ m_className + std::to_string(_tag)+"_IS_IN");
        _eventDispatcher->removeCustomEventListeners("NODE_"+ m_className + std::to_string(_tag)+"_IS_IN");
        
        auto mainObject = MainObject::getInstance();
        if(mainObject->isAllNodeIsIn())
        {
            _eventDispatcher->dispatchCustomEvent("ALL_NODE_IS_IN");
        }
    });
    auto seq = Sequence::create(delay, spawn, callFunc, NULL);
    runActionSeq("add", seq, speedFactor, delayFactor);
}
void KFNode::remove(float speedFactor, float delayFactor, std::string movement, bool create)
{
    m_isIn = false;
    
    float speedRemove = speedFactor;
    float speedAdd = speedFactor;
    float delayRemove = delayFactor;
    float delayAdd = delayFactor;
    float scaleFactor = 0.8;
    float randomTime = MainGrid::getRandom(this->getTag());
    float endDelayTime = 0;
    float createDelayTime = 0;
    if (create)
    {
        createDelayTime = 0;
    }
    if(movement == "remove")
    {
        speedRemove = speedFactor;
        delayRemove = 0;
        scaleFactor = 1;
    }
    if(movement == "add")
    {
        delayAdd = 0;
        speedAdd = speedFactor;
        const float randomTimeMax = 1;
        endDelayTime = randomTimeMax * delayRemove - randomTime * delayRemove;
    }
    
    float delayTime = randomTime * delayRemove * delayFactor;
    auto delay = DelayTime::create(delayTime);
    auto endDelay = DelayTime::create(endDelayTime);
    auto createDelay = DelayTime::create(createDelayTime);
    auto move = MoveTo::create(m_moveTime * speedRemove, Vec2(m_position.x, m_position.y - m_moveInOutRange));
    auto moveOut = EaseBackIn::create(move->clone());
    auto scale = ScaleTo::create(0.7, scaleFactor * m_isFlipped, scaleFactor);
    auto spawn = Spawn::createWithTwoActions(moveOut, scale);
    
    auto isOutFunc = CallFunc::create([=]()
    {
        finishAction("remove");
        m_isOut = true;
        _eventDispatcher->dispatchCustomEvent("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT");
        _eventDispatcher->removeCustomEventListeners("NODE_"+ m_className + std::to_string(_tag)+"_IS_OUT");
    });
    auto callFunc = CallFunc::create([=]()
    {
        if(movement == "both")
        {
            setScale(0.8 * m_isFlipped, 0.8);
            add(speedAdd, delayAdd);
        }
        else if(movement == "add")
        {
            setScale(1 * m_isFlipped, 1);
            add(speedAdd, delayAdd);
        }
        else if(movement == "remove")
        {
            setScale(0.8 * m_isFlipped, 0.8);
            add();
        }
    });
    auto seq  = Sequence::create(delay, spawn, endDelay, isOutFunc, createDelay, callFunc,  NULL);
    runActionSeq("remove", seq, speedFactor, delayFactor, movement);
}

//-------------------------------PLACE UP DOWN-----------------------------------
void KFNode::place(float speedFactor, bool isCascadedMove)
{
    if(!m_isPlace)
    {
        m_cascadedMove = isCascadedMove;
        auto move = MoveTo::create((m_moveTime / 2) * speedFactor, Vec2(m_position.x, m_position.y));
        auto callFunc  = CallFunc::create([this]()
        {
            finishAction("place");
            m_isPlace = true;
            _eventDispatcher->dispatchCustomEvent("NODE_"+ m_className + std::to_string(_tag)+"_IS_PLACE");
            _eventDispatcher->removeCustomEventListeners("NODE_"+ m_className + std::to_string(_tag)+"_IS_PLACE");

        });
        auto seq  = Sequence::create(move, callFunc, NULL);
        runActionSeq("place", seq, speedFactor);
        m_cascadedMove = true;
    }
}
void KFNode::up(float speedFactor, bool isCascadedMove)
{
    m_isPlace = false;
    m_cascadedMove = isCascadedMove;
    auto move = MoveTo::create((m_moveTime / 2) * speedFactor, Vec2(m_position.x, m_position.y + m_moveRange));
    auto callFunc  = CallFunc::create([this]()
    {
        finishAction("up");
        _eventDispatcher->dispatchCustomEvent("NODE_"+ m_className + std::to_string(_tag)+"_IS_UP");
        _eventDispatcher->removeCustomEventListeners("NODE_"+ m_className + std::to_string(_tag)+"_IS_UP");
    });
    auto seq  = Sequence::create(move, callFunc, NULL);
    runActionSeq("up", seq, speedFactor);
    m_cascadedMove = true;
}
void KFNode::down(float speedFactor, bool isCascadedMove)
{
    m_isPlace = false;
    m_cascadedMove = isCascadedMove;
    auto move = MoveTo::create((m_moveTime / 2) * speedFactor, Vec2(m_position.x, m_position.y - m_moveRange));
    auto callFunc = CallFunc::create([this]()
    {
        finishAction("down");
        _eventDispatcher->dispatchCustomEvent("NODE_"+ m_className + std::to_string(_tag)+"_IS_DOWN");
        _eventDispatcher->removeCustomEventListeners("NODE_"+ m_className + std::to_string(_tag)+"_IS_DOWN");
    });
    auto seq = Sequence::create(move, callFunc, NULL);
    runActionSeq("down", seq, speedFactor);
    m_cascadedMove = true;
}

//-------------------------------------MOVE ACTION------------------------------
void KFNode::runActionSeq(std::string action, cocos2d::Sequence* seq, float speedFactor, float delayFactor, std::string movement)
{
    if(action != m_currentAction && m_currentAction != "remove")
    {
        m_isMove = true;
        m_currentAction = action;
        
        ssize_t nAction = this->getActionManager()->getNumberOfRunningActionsInTarget(this);
        if(nAction > 0)
        {
            this->getActionManager()->removeAction(m_runningSeq);
        }
        m_runningSeq = seq->clone();
        this->runAction(m_runningSeq);
    
        auto nodeList = MainObject::getMainLayer()->getChildren();
        Vector<Node*>::iterator nlIt;
        for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
        {
            auto node = dynamic_cast<KFNode*>(*nlIt);
            if(node &&
               node->getMetaName() != m_metaName &&
               node->getTag() == _tag &&
               node->getCurrentAction() != "remove" &&
               m_cascadedMove == true)
            {
                node->runActionName(action, speedFactor, delayFactor, movement);
            }
        }
    }
}
void KFNode::runActionName(std::string action, float speedFactor, float delayFactor, std::string movement)
{
    if(action == "add"){ add(speedFactor, delayFactor);}
    else if(action == "remove"){remove(speedFactor, delayFactor, movement);}
    else if(action == "place"){ place(speedFactor);}
    else if(action == "up"){ up(speedFactor);}
    else if(action == "down"){ down(speedFactor);}
}
void KFNode::finishAction(std::string action)
{
    if(action == m_currentAction)
    {
        m_currentAction = "";
        m_isMove = false;
        
        if(!MainObject::isNodesMove())
        {
            _eventDispatcher->dispatchCustomEvent("NO_NODE_MOVE");
            _eventDispatcher->removeCustomEventListeners("NO_NODE_MOVE");
        }
    }
}

//---------------------------------SHADERS------------------------------
bool KFNode::addShader()
{
    
    
    return true;
}

//---------------------------------RETURN FUNCTION------------------------------
bool KFNode::getIsIn()
{
    return m_isIn;
}
bool KFNode::getIsOut()
{
    return m_isOut;
}
void KFNode::setIsMove(bool isMove)
{
    m_isMove = isMove;
}
bool KFNode::getIsMove()
{
    return m_isMove;
}
bool KFNode::getIsPlace()
{
    return m_isPlace;
}
bool KFNode::getIsSelect()
{
    return m_select;
}

std::string KFNode::getCurrentAction()
{
    return m_currentAction;
}

std::map<std::string, int> KFNode::getPositionLCI()
{
    std::map<std::string, int> positionLCI;
    positionLCI["line"] = m_line;
    positionLCI["collumn"] = m_collumn;
    positionLCI["index"] = m_index;
    positionLCI["x"] = m_position.x;
    positionLCI["y"] = m_position.x;
    
    return positionLCI;
}

std::string KFNode::getClassName()
{
    return m_className;
}

int KFNode::getIndexClassName()
{
    int index = 0;
    if(m_className != "box")
    {
        index = 2;
    }
    return index;
}

void KFNode::setMetaName(std::string metaName)
{
    m_metaName = metaName;
}
std::string KFNode::getMetaName()
{
    return m_metaName;
}

void KFNode::setNumber(int number)
{
    m_number = number;
}
int KFNode::getNumber()
{
    return m_number;
}

Node* KFNode::getDisplayNode()
{
    Node* displayNode = this->getChildByName("displayNode");
    
    return displayNode;
}
