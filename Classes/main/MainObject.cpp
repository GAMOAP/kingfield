//
//  MainObject.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 30/05/2019.
//

#include "MainObject.hpp"

#include "MainDirector.hpp"
#include "MainInfoLayer.hpp"

#include <iostream>

USING_NS_CC;

MainObject* MainObject::getInstance()
{
    if (!m_SharedMainObject)
    {
        m_SharedMainObject = new (std::nothrow) MainObject;
        m_SharedMainObject->init();
    }
    
    return m_SharedMainObject;
}

bool MainObject::init()
{
    
    return true;
}
//Layer function....................................................
void MainObject::setMainLayer(cocos2d::Node* mainLayer)
{
    m_SharedMainObject->m_mainLayer = mainLayer;
}
cocos2d::Node* MainObject::getMainLayer()
{
    return m_SharedMainObject->m_mainLayer;
}
void MainObject::setInfoLayer(cocos2d::Node* infoLayer)
{
    m_SharedMainObject->m_infoLayer = infoLayer;
    MainInfoLayer::getInstance();
}
cocos2d::Node* MainObject::getInfoLayer()
{
    return m_SharedMainObject->m_infoLayer;
}

//Node function......................................................
bool MainObject::isAllNodeIsIn()
{
    bool allNodeIsIn = true;
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto node = dynamic_cast<KFNode*>(*scIt);
        if(node && !node->getIsIn())
        {
            allNodeIsIn = false;
        }
    }
    return allNodeIsIn;
}
bool MainObject::isNodesMove()
{
    bool nodesMove = false;
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto node = dynamic_cast<KFNode*>(*scIt);
        if(node && node->getIsMove())
        {
            nodesMove = true;
        }
    }
    return nodesMove;
}
KFNode* MainObject::getNodeByTag(int tag, bool noBox)
{
    KFNode* node = nullptr;
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto nodeTest = dynamic_cast<KFNode*>(*scIt);
        auto boxTest = dynamic_cast<Box*>(*scIt);
        if(nodeTest && nodeTest->getTag() == tag)
        {
            if(noBox && !boxTest)
                node = nodeTest;
        }
    }
    return node;
}
int MainObject::getNumberNodeByTag(int tag)
{
    int nNode = 0;
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto nodeTest = dynamic_cast<KFNode*>(*scIt);
        if(nodeTest && nodeTest->getTag() == tag)
            nNode++;
    }
    return nNode;
}

//Boxes function......................................................
Box* MainObject::getBoxByLC(int line, int collumn)
{
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto box = dynamic_cast<Box*>(*scIt);
        if(box){
            auto position = box->getPositionLCI();
            if(position["line"] == line && position["collumn"] == collumn)
            {
                return box;
            }
        }
    }
    return nullptr;
}
Box* MainObject::getBoxByTag(int tag)
{
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto box = dynamic_cast<Box*>(*scIt);
        if(box){
            auto boxNumber = box->getTag();
            if(boxNumber == tag)
            {
                return box;
            }
        }
    }
    return nullptr;
}

//Charaters function......................................................
Character* MainObject::getCharByNumber(int number)
{
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto character = dynamic_cast<Character*>(*scIt);
        if(character && character->getNumber() == number)
        {
            return character;
        }
    }
    return nullptr;
}
Character* MainObject::getCharByTag(int tag)
{
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto character = dynamic_cast<Character*>(*scIt);
        if(character && character->getTag() == tag)
        {
            return character;
        }
    }
    return nullptr;
}

//CharacterUI function..............................................
CharacterUI* MainObject::getCharacterUI()
{
    
}

//Card function......................................................
Card* MainObject::getCardByNumber(int number, std::string board)
{
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto card = dynamic_cast<Card*>(*scIt);
        if(card && card->getNumber() == number && card->getBoard() == board)
        {
            return card;
        }
    }
    return nullptr;
}
Card* MainObject::getCardByTag(int tag)
{
    auto sceneChildren = m_SharedMainObject->m_mainLayer->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = sceneChildren.begin(); scIt != sceneChildren.end(); scIt++)
    {
        auto card = dynamic_cast<Card*>(*scIt);
        if(card && card->getTag() == tag)
        {
            return card;
        }
    }
    return nullptr;
}
