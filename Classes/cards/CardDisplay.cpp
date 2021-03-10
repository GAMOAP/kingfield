//
//  CardDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 12/07/2019.
//

#include "MainStuff.hpp"

#include "GameCharacters.hpp"

#include "CardDisplay.hpp"

#include "KFSprite.hpp"

#include <iostream>

USING_NS_CC;

CardDisplay* CardDisplay::create(std::string type, std::string breed, std::string object, std::string board)
{
    CardDisplay* ret = new (std::nothrow) CardDisplay();
    if(ret && ret->setTexture(type, breed, object))
    {
        ret->m_board = board;
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

bool CardDisplay::setTexture(std::string type, std::string breed, std::string object, bool popUpActived)
{
    setName("displayNode");
    
    m_type = type;
    m_breed = breed;
    m_object = object;
    
    m_popUpActived = popUpActived;
    
    setDisplay();
    
    return true;
}

void CardDisplay::setDisplay()
{
    setCascadeColorEnabled(true);
    
    setImage();
    setSlots();
    
    if(m_type == "move" || m_type == "spell" || m_type == "weapon" || m_type == "object")
    {
        setMana();
        m_mana->setVisible(true);
        setChessBoard();
        m_chessBoard->setVisible(true);
    }
    else
    {
        if(m_mana)
            m_mana->setVisible(false);
        if(m_chessBoard)
            m_chessBoard->setVisible(false);
    }
}

void CardDisplay::setImage()
{
    std::string imageFileName = KFSprite::getFile(m_breed + m_object + "_" + m_type + "Card");
    if(!m_image)
    {
        m_image = Sprite::create(imageFileName);
        this->addChild(m_image, 0);
    }
    else
    {
        m_image->setTexture(imageFileName);
    }
    
    if((m_type == "move" || m_type == "spell" || m_type == "weapon" || m_type == "object") && !isCardAvailable())
            m_image->setColor(Color3B::GRAY);
        else
            m_image->setColor(Color3B::WHITE);
}
void CardDisplay::setMana()
{
    std::string cardCrystalColor = "red";
    if(isCardAvailable())
    {
        if(isCardChanged() && m_board != "library")
            cardCrystalColor = "green";
        else
            cardCrystalColor = "blue";
    }
    
    std::string manaFileName = KFSprite::getFile("cardUI_card_crystal_" + cardCrystalColor);
    if(!m_mana)
    {
        m_mana = Sprite::create(manaFileName);
        m_mana->setAnchorPoint(Vec2(0.5, 0.5));
        m_mana->setPosition(m_manaPosition);
        m_mana->setCascadeOpacityEnabled(true);
        this->addChild(m_mana, 1);
    }
    
    bool getOrigin = false;
    if(m_board == "library")
        getOrigin = true;
    
    int manaNbr = MainStuff::getCardSpec(m_type, m_breed, m_object)->getMana(getOrigin);
    std::string manaNbrFileName = KFSprite::getFile("cardUI_number_" + std::to_string(manaNbr));
    if(!m_manaNbr)
    {
        m_manaNbr = Sprite::create(manaNbrFileName);
        m_manaNbr->setName(manaNbrFileName);
        m_manaNbr->setAnchorPoint(Vec2(0.5, 0.5));
        m_manaNbr->setPosition(m_manaNbrPosition);
        m_mana->addChild(m_manaNbr);
    }
    
    if(m_mana && m_manaNbr)
    {
        if(m_popUpActived && m_manaNbr->getName() != manaNbrFileName)
        {
            float popUpTime = 0.2;
            auto scaleUp = ScaleTo::create(popUpTime, 2.5);
            auto scaleDown = ScaleTo::create(popUpTime, 1);
            auto scaleUpEase = EaseOut::create(scaleUp, 0.5);
            auto scaleDownEase = EaseIn::create(scaleDown, 0.5);
            auto callFunc = CallFunc::create([=]()
            {
                m_mana->setTexture(manaFileName);
                
                m_manaNbr->setTexture(manaNbrFileName);
                m_manaNbr->setName(manaNbrFileName);
            });
            auto popUpSeq = Sequence::create(scaleUpEase, callFunc, scaleDownEase, NULL);
            m_mana->runAction(popUpSeq);
        }
        else
        {
            m_mana->setTexture(manaFileName);
            
            m_manaNbr->setName(manaNbrFileName);
            m_manaNbr->setTexture(manaNbrFileName);
        }
    }
}
void CardDisplay::setSlots()
{
    if(!m_slotContener)
    {
        m_slotContener = Node::create();
        m_slotContener->setAnchorPoint(Vec2(0, 0));
        m_slotContener->setPositionY(slotStartY);
        m_slotContener->setCascadeOpacityEnabled(true);
        this->addChild(m_slotContener, 0);
    }
    int positionX = slotStartX;
    
    for(int s = 0; s < m_nbrSlot ; s++)
    {
        auto slot = m_slotList[s];
        std::string slotName = MainStuff::getCardSpec(m_type, m_breed, m_object)->getSlot("slot" + std::to_string(s));
        std::string slotFileName = KFSprite::getFile("cardUI_" + slotName);
        if(!slot)
        {
            slot = Sprite::create(slotFileName);
            slot->setAnchorPoint(Vec2(0, 0));
            slot->setPositionX(slotDec * s);
            m_slotContener->addChild(slot, 0);
            m_slotList[s] = slot;
        }
        else
        {
            slot->setTexture(slotFileName);
        }
        if(slotName != "NULL")
        {
            slot->setVisible(true);
        }
        else
        {
            slot->setVisible(false);
            positionX += slotDec/2;
        }
    }
    m_slotContener->setPositionX(positionX);
}
void CardDisplay::setChessBoard()
{
    std::vector<std::vector<int>> board = MainStuff::getCardSpec(m_type, m_breed, m_object)->getBoard();
    
    std::string checkBoardFileName = KFSprite::getFile("cardUI_chess_board");
    if(!m_chessBoard)
    {
        m_chessBoard = Sprite::create(checkBoardFileName);
        m_chessBoard->setPosition(m_boardOrigin);
        m_chessBoard->setCascadeOpacityEnabled(true);
        this->addChild(m_chessBoard, 1);
    }
    
    auto children = m_chessBoard->getChildren();
    Vector<Node*>::iterator scIt;
    for(scIt = children.begin(); scIt != children.end(); scIt++)
    {
        auto node = *scIt;
        if(node)
        {
            node->removeFromParent();
        }
    }
    
    for(int i = 0; i < board.size(); i++)
    {
        std::vector<int> boardSpec = board[i];
        int originX = boardSpec[0] * m_boardDec + m_boardMoveOrigin.x;
        int originY = boardSpec[1] * m_boardDec + m_boardMoveOrigin.y;
        Color3B boardColor = boardMoveColor[boardSpec[2]];
        
        for(int y = 3; y < boardSpec.size(); y++)
        {
            std::string boardMoveFileName = KFSprite::getFile("cardUI_board_" + std::to_string(boardSpec[y]));
            auto boardMove = Sprite::create(boardMoveFileName);
            boardMove->setAnchorPoint(Vec2(0.5, 0.5));
            boardMove->setPosition(Vec2(originX, originY));
            boardMove->setColor(boardColor);
            m_chessBoard->addChild(boardMove);
        }
        for(int w = 3; w < boardSpec.size(); w++)
        {
            std::string overBoardMoveFileName = KFSprite::getFile("cardUI_over_board_" + std::to_string(boardSpec[w]));
            auto overboardMove = Sprite::create(overBoardMoveFileName);
            overboardMove->setAnchorPoint(Vec2(0.5, 0.5));
            overboardMove->setPosition(Vec2(originX, originY));
            m_chessBoard->addChild(overboardMove, 1);
        }
    }
    if(board.size() > 0){
        std::string yellowDotFileName = KFSprite::getFile("cardUI_board_0");
        auto yellowDot = Sprite::create(yellowDotFileName);
        yellowDot->setAnchorPoint(Vec2(0.5, 0.5));
        yellowDot->setPosition(m_boardMoveOrigin);
        m_chessBoard->addChild(yellowDot, 2);
        
        std::string overYellowDotFileName = KFSprite::getFile("cardUI_over_board_0");
        auto overYellowDot = Sprite::create(overYellowDotFileName);
        overYellowDot->setAnchorPoint(Vec2(0.5, 0.5));
        overYellowDot->setPosition(m_boardMoveOrigin);
        overYellowDot->setColor(boardMoveColor[board[0][2]]);
        m_chessBoard->addChild(overYellowDot, 2);
    }
}

void CardDisplay::setSelect()
{
    
}
void CardDisplay::setUnselect()
{
    
}

void CardDisplay::flipChessBoard(bool isFlipped)
{
    int intIsFlip = (-1 + isFlipped * 2)* -1;
    if(m_chessBoard)
        m_chessBoard->setScaleX(intIsFlip);
}

bool CardDisplay::isCardAvailable()
{
    bool cardAvailable = false;
    
    auto character = GameCharacters::getCharSelect();
    int charSelectNbr = 2;
    if(character){charSelectNbr = character->getNumber();}
    const int charCrystal = MainStuff::getCharSpec(charSelectNbr)["crystal"];
    const int charCrystalRed = MainStuff::getCharSpec(charSelectNbr)["crystal_red"];
    
    bool getOrigin = false;
       if(m_board == "library")
           getOrigin = true;
    int cardCost = MainStuff::getCardSpec(m_type, m_breed, m_object)->getMana(getOrigin);
    
    if(charCrystal >= cardCost && charCrystal > charCrystalRed)
        cardAvailable = true;
    
    return cardAvailable;
}
bool CardDisplay::isCardChanged()
{
    int manaOrigin = MainStuff::getCardSpec(m_type, m_breed, m_object)->getMana(true);
    int mana = MainStuff::getCardSpec(m_type, m_breed, m_object)->getMana(false);
    if(manaOrigin != mana)
        return true;
    else
        return false;
}

std::string CardDisplay::getMetaName()
{
    std::string metaName = m_type + m_breed + m_object;
    
    return metaName;
}
