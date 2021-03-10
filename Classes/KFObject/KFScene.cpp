//
//  KFScene.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 24/09/2019.
//

#include "KFScene.hpp"
#include "MainInfoLayer.hpp"

#include "MainObject.hpp"

#include "LibraryCard.hpp"

bool KFScene::init()
{
    
    return true;
}
void KFScene::addToStage()
{
    
}
void KFScene::removeToStage()
{
    
}

bool KFScene::allNodeIsIn()
{
    return true;
}

bool KFScene::appConnecting()
{
    MainInfoLayer::addConnectSprite();
    return true;
}
bool KFScene::appConnected()
{
    MainInfoLayer::removeConnectSprite();
    return true;
}

bool KFScene::touchBox(int tag)
{
    return true;
}

bool KFScene::unTouchBox(int tag)
{
    return true;
}

bool KFScene::cancelTouchBox(int tag)
{
    return true;
}

bool KFScene::longTouchBox(int tag)
{
    return true;
}

bool KFScene::getIsPlayerTurn()
{
    return true;
}
bool KFScene::getIsTeamTurn(int charNbr)
{
    return true;
}

void KFScene::setName(std::string sceneName)
{
    m_name = sceneName;
}
std::string KFScene::getName()
{
    return m_name;
}

//-----------------------TOUCH OBJECTS----------------------------
bool KFScene::getIsUsedBox(int tag)
{
    int nNode = MainObject::getNumberNodeByTag(tag);
    if(nNode > 1)
        return true;
    else
        return false;
}
void KFScene::setTouchObject(int tag)
{
    KFNode* object = nullptr;
    if(!getIsUsedBox(tag))
    {
        object = MainObject::getBoxByTag(tag);
    }
    else
    {
        object = MainObject::getNodeByTag(tag, true);
    }
    
    bool isCascadedMove = true;
    auto libraryNode = dynamic_cast<LibraryCard*>(object);
    if(libraryNode)
        isCascadedMove = false;
    
    if(object)
    {
        m_touchedObjectTag = tag;
        if(object->getIsSelect())
        {
            object->place(1, isCascadedMove);
        }
        else
        {
            object->down(1, isCascadedMove);
        }
    }
}
void KFScene::cancelTouchObject()
{
    auto tag = m_touchedObjectTag;
    if(tag)
    {
        KFNode* object = nullptr;
        if(!getIsUsedBox(tag))
            object = MainObject::getBoxByTag(tag);
        else
            object = MainObject::getNodeByTag(tag, true);
        
        bool isCascadedMove = true;
        auto libraryNode = dynamic_cast<LibraryCard*>(object);
        if(libraryNode)
            isCascadedMove = false;
        
        if(object)
        {
            if(object->getIsSelect())
                object->up(1, isCascadedMove);
            else
                object->place(1, isCascadedMove);
        }
        m_touchedObjectTag = NAN;
    }
}
