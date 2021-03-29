//
//  MainAction.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 23/08/2019.
//

#include "Constants.h"
#include "KFSpecCard.hpp"

#include "MainStuff.hpp"
#include "MainObject.hpp"

#include "MainAction.hpp"

#include <iostream>

USING_NS_CC;

std::vector<Vec3> MainAction::getActionBoxesTags(int charSelectNbr, int cardSelectNbr)
{
    std::vector<std::string> cardName = MainStuff::getStuffByName(charSelectNbr, cardSelectNbr);
    KFSpecCard* specCard = MainStuff::getCardSpec(CARD_TYPE[cardSelectNbr], cardName[0], cardName[1]);
    
    std::vector<Vec3> actionBoxesTags;
    std::vector<std::vector<int>> board = specCard->getBoard();
    
    const int charCrystal = MainStuff::getCharSpec(charSelectNbr)["crystal"];
    const int charCrystalRed = MainStuff::getCharSpec(charSelectNbr)["crystal_red"];
    const int cardCost = specCard->getMana(false);
    
    if(cardCost <= charCrystal && charCrystal > charCrystalRed )
    {
        std::vector<int>forbidenTagList = setForbiddenTagList(charSelectNbr, board);
        for(int b = 0; b < board.size(); b++)
        {
            std::vector<int> boardAction = board[b];
            const int startTag = getStartTag(charSelectNbr, boardAction[0], boardAction[1]);
            int actionType = boardAction[2];
            
            for(int m = 3; m < boardAction.size(); m++)
            {
                int tag = startTag + getBoardMap(charSelectNbr)[boardAction[m]];
                switch (actionType) {
                        // type move.
                    case 0:
                        if(isInBoardTag(tag) && !isCharTag(tag) && !isForbiddenTag(tag, forbidenTagList))
                            actionBoxesTags.push_back(Vec3(startTag, tag, actionType));
                        break;
                        // type strike.
                    case 1:
                        if(isInBoardTag(tag) && isCharTag(tag))
                            actionBoxesTags.push_back(Vec3(startTag, tag, actionType));
                        break;
                        // type spell.
                    case 2:
                        if(isInBoardTag(tag) && isCharTag(tag))
                            actionBoxesTags.push_back(Vec3(startTag, tag, actionType));
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    
    return actionBoxesTags;
}

std::vector<KFAction*> MainAction::getActionSequence(int charSelectNbr, int cardSelectNbr, int touchedBoxTag, uint sRandom)
{
    srand(sRandom);
    
    std::vector<std::string> cardName = MainStuff::getStuffByName(charSelectNbr, cardSelectNbr);
    KFSpecCard* specCard = MainStuff::getCardSpec(CARD_TYPE[cardSelectNbr], cardName[0], cardName[1]);
    int crystalCost = specCard->getMana(false);
    
    std::vector<KFAction*> actionSequence;
    std::vector<Vec3> actionBoxesTags = getActionBoxesTags(charSelectNbr, cardSelectNbr);
    
    Vec3 touchedBox;
    for(int t = 0; t < actionBoxesTags.size(); t++)
    {
        if(actionBoxesTags[t].y == touchedBoxTag)
            touchedBox = actionBoxesTags[t];
    }
    int actionType = touchedBox.z;
    
    //move.
    if(actionType == 0)
    {
        int max = 0;
        int testTag = touchedBox.y;
        while(testTag != charSelectNbr && max < 9)
        {
            for(int a = 0; a < actionBoxesTags.size(); a++)
            {
                int startTag = actionBoxesTags[a].x;
                int endTag = actionBoxesTags[a].y;
                
                if(testTag == endTag)
                {
                    auto action = KFAction::setAction(actionType, charSelectNbr);
                    action->setCost(crystalCost);
                    action->setStartTag(startTag);
                    action->setEndTag(endTag);
                    actionSequence.insert(actionSequence.begin(), action);
                    testTag = startTag;
                }
            }
            max++;
        }
    }
    
    //strike.
    if(actionType == 1)
    {        
        auto mainStuff = MainStuff::getInstance();
        
        for(int s = 0; s < 3; s++)
        {
            std::string slotName = specCard->getSlot(s);
            
            if(slotName != "NULL")
            {
                auto action = KFAction::setAction(actionType, charSelectNbr);
                action->setCost(crystalCost);
                
                int attack = mainStuff->getCharSpec(charSelectNbr)["strike"] + 1;
                
                int charForce = rand() % (attack);
                action->setCharAttackForce(charForce);
                action->setSlotSpec(slotName);
                
                std::vector<std::vector<int>> strikedList;
                
                for(int a = 0; a < actionBoxesTags.size(); a++)
                {
                    if(touchedBox.x == actionBoxesTags[a].x)
                    {
                        int charStrikedNbr = MainObject::getInstance()->getCharByTag(actionBoxesTags[a].y)->getNumber();
                        int charForce = rand() % (mainStuff->getCharSpec(charStrikedNbr)["shield"] + 1);
                        std::vector<int> strikeValue = {charStrikedNbr, charForce};
                        strikedList.push_back(strikeValue);
                        
                        action->setCharStrikedList(strikedList);
                    }
                }
                actionSequence.push_back(action);
            }
        }
    }
    
    //spell.
    if(actionType == 2)
    {
        
    }
    
    //init random after random was controled
    srand((unsigned)time(NULL));
    
    return actionSequence;
}
std::vector<KFAction*> MainAction::getEnemyActionSequence(int charSelectNbr, int cardSelectNbr, int touchedBoxTag, uint sRandom)
{
    std::vector<KFAction*> actionSequence;
    
    int charEnemyNbr = charSelectNbr + 5;
    int l = floor(touchedBoxTag / 10);
    int c = (touchedBoxTag - l * 10);
    int enemyTouchedBoxTag = (-l + 8) * 10 - c + 6;
    
    actionSequence = getActionSequence(charEnemyNbr, cardSelectNbr, enemyTouchedBoxTag, sRandom);
    
    return actionSequence;
}

bool MainAction::isInBoardTag(int tag)
{
    bool isInBoard = false;
    for(int b = 0; b < m_boardIn.size(); b++)
    {
        if(tag == m_boardIn[b])
            isInBoard = true;
    }
    return isInBoard;
}
bool MainAction::isCharTag(int tag)
{
    auto character = MainObject::getCharByTag(tag);
    if(!character)
        return false;
    
    return true;
}
std::vector<int> MainAction::setForbiddenTagList(int charNbr, std::vector<std::vector<int>> board)
{
    std::vector<int> forbiddenTagList;
    
    int charTag = MainObject::getCharByNumber(charNbr)->getTag();
    
    for(int b = 0; b < board.size(); b++)
    {
        std::vector<int> boardAction = board[b];
        const int startTag = getStartTag(charNbr, boardAction[0], boardAction[1]);
        
        if((isCharTag(startTag) && startTag != charTag) || isForbiddenTag(startTag, forbiddenTagList))
        {
            for(int m = 3; m < boardAction.size(); m++)
            {
                int tag = startTag + getBoardMap(charNbr)[boardAction[m]];
                forbiddenTagList.push_back(tag);
            }
        }
    }
    return forbiddenTagList;
}
bool MainAction::isForbiddenTag(int tag, std::vector<int> forbiddenTagList)
{
    bool isForbiddenTag = false;
    for(int f = 0; f < forbiddenTagList.size(); f++)
    {
        if(tag == forbiddenTagList[f])
            isForbiddenTag = true;
    }
    return isForbiddenTag;
}

//team invert box(right, left).
int MainAction::getStartTag(int charNbr, int boardActionC, int boardActionL)
{
    int startTag;
    int charTag = MainObject::getCharByNumber(charNbr)->getTag();
    if(charNbr < 5)
        startTag = charTag + boardActionC + boardActionL*10;
    else
        startTag = charTag + boardActionC - boardActionL*10;
    return startTag;
}

std::vector<int> MainAction::getBoardMap(int charNbr)
{
    std::vector<int> boardMap;
    
    if(charNbr < 5)
        boardMap = m_friendBoardMap;
    else
        boardMap = m_enemyBoardMap;
    
    return boardMap;
}
