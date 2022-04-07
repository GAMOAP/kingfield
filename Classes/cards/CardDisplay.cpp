//
//  CardDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 12/07/2019.
//

/*
#include "MainStuff.hpp"
#include "MainUser.hpp"

#include "GameCharacters.hpp"
*/

#include "MainUser.hpp"
#include "MainStuff.hpp"

#include "GameCharacters.hpp"

#include "CardDisplay.hpp"

#include <iostream>

USING_NS_CC;

CardDisplay* CardDisplay::create(std::string type, std::string breed, std::string object, std::string board)
{
    CardDisplay* ret = new (std::nothrow) CardDisplay();
    if(ret && ret->setTexture(type, breed, object, board, NORMAL))
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

bool CardDisplay::setTexture(std::string type, std::string breed, std::string object, std::string board, TextureStyle style)
{
    setName("displayNode");
    
    //init card display attributes
    m_type = type;
    m_breed = breed;
    m_object = object;
    m_board = board;
    m_style = style;
    
    Character* character = GameCharacters::getCharSelect();
    m_specCard = MainStuff::getCardSpec(m_type, m_breed, m_object, character->getNumber());
    
    //specCard before player chose what card level up
    if(m_style == POPLEVELUP)
    {
        m_specCard = MainStuff::getCardSpec(m_type, m_breed, m_object, character->getNumber(), true);
    }
    
    setDisplay();
    
    return true;
}

void CardDisplay::setDisplay()
{
    setCascadeColorEnabled(true);
    
    setImage();
    setSlots();
    
    if(m_board == "library")
    {
        setLeft();
    }
    else
    {
        if(m_leftPointConteneur)
        {
            m_leftPointConteneur->removeFromParent();
        }
    }
    
    if(m_type == "move" || m_type == "spell" || m_type == "weapon" || m_type == "object")
    {
        //mana
        if(!m_manaSprite || m_mana != m_specCard->getMana(false))
        {
            if((m_board == "deck" && m_style == POPLEVELUP) || m_style == POPUP)
            {
                popUp(MANA);
            }
            else
            {
                setMana();
            }
        }
        
        m_manaSprite->setVisible(true);
        
        //slots
        for(int s = 0; s > m_nbrSlot; s++)
        {
            if(m_slotList[s] && m_board == "deck" && m_style == POPLEVELUP)
            {
                popUp(SLOTS);
            }
        }
        
        //chess board
        setChessBoard();
        m_chessBoardSprite->setVisible(true);
    }
    else
    {
        if(m_manaSprite)
        {
            m_manaSprite->setVisible(false);
        }
            
        if(m_chessBoardSprite)
        {
            m_chessBoardSprite->setVisible(false);
        }
    }
}

//-------------------------------IMAGE---------------------------
void CardDisplay::setImage()
{
    if(m_imageSprite && (m_style == LEVELUP || m_style == POPLEVELUP))
    {
        int animationImageNbr = 4;
        cocos2d::Vector<cocos2d::SpriteFrame*> imageFrame;
        imageFrame.reserve(animationImageNbr);
        auto spriteCache = SpriteFrameCache::getInstance();
        for(int f = 0; f < animationImageNbr; f++)
        {
            std::string animName = "UI/card/level_up_btn_up_" + std::to_string(f) + ".png";
            imageFrame.pushBack(spriteCache->getSpriteFrameByName(animName.c_str()));
        }
        Animation* imageAnimation = Animation::createWithSpriteFrames(imageFrame, 0.1f);
        Animate* imageAnimate = Animate::create(imageAnimation);
        m_imageSprite->runAction(RepeatForever::create(imageAnimate));
    }
    else
    {
        std::string imageFileName = "card/" + m_breed + "/" + m_object + "_" + m_type + "Card.png";
        if(!m_imageSprite)
        {
            m_imageSprite = Sprite::createWithSpriteFrameName(imageFileName);
            this->addChild(m_imageSprite, 1);
            
            m_imageSprite->stopAllActions();
        }
        else
        {
            m_imageSprite->stopAllActions();
            m_imageSprite->setSpriteFrame(imageFileName);
        }
        
        if((m_type == "move" || m_type == "spell" || m_type == "weapon" || m_type == "object") && !isCardAvailable())
        {
            m_imageSprite->setColor(Color3B::GRAY);
        }
        else
        {
            m_imageSprite->setColor(Color3B::WHITE);
        }
    }
    
}

//-------------------------------MANA---------------------------
void CardDisplay::setMana()
{
    if(!m_manaLayer)
    {
        m_manaLayer = Node::create();
        m_manaLayer->setPosition(m_manaPosition);
        m_manaLayer->setCascadeOpacityEnabled(true);
        this->addChild(m_manaLayer, 2);
    }
    
    std::string cardCrystalColor = "red";
    if(isCardAvailable())
    {
        if(isCardChanged() && m_board != "library")
            cardCrystalColor = "green";
        else
            cardCrystalColor = "blue";
    }
    
    std::string manaFileName = "UI/card/card_crystal_" + cardCrystalColor + ".png";
    if(!m_manaSprite)
    {
        m_manaSprite = Sprite::createWithSpriteFrameName(manaFileName);
        m_manaLayer->addChild(m_manaSprite);
    }
    
    bool getOrigin = false;
    if(m_board == "library")
        getOrigin = true;
    
    int manaNbr = m_specCard->getMana(getOrigin);
    std::string manaNbrFileName = "UI/card/number_" + std::to_string(manaNbr) + ".png";
    if(!m_manaNbrSprite)
    {
        m_manaNbrSprite = Sprite::createWithSpriteFrameName(manaNbrFileName);
        m_manaNbrSprite->setPosition(m_manaNbrPosition);
        
        m_manaLayer->addChild(m_manaNbrSprite);
    }
    
    if(m_manaSprite && m_manaNbrSprite)
    {
        m_manaSprite->setSpriteFrame(manaFileName);
        m_manaNbrSprite->setSpriteFrame(manaNbrFileName);
        m_mana = m_specCard->getMana(false);
    }
}

//-------------------------------SLOTS---------------------------
void CardDisplay::setSlots()
{
    if(!m_slotContener)
    {
        m_slotContener = Node::create();
        m_slotContener->setAnchorPoint(Vec2(0, 0));
        m_slotContener->setPositionY(m_slotStartY);
        m_slotContener->setCascadeOpacityEnabled(true);
        this->addChild(m_slotContener, 2);
    }
    int positionX = m_slotStartX;
    
    for(int s = 0; s < m_nbrSlot ; s++)
    {
        auto slot = m_slotList[s];
        std::string slotName = m_specCard->getSlot("slot" + std::to_string(s));
        std::string slotFileName = "UI/card/" + slotName + ".png";
        if(!slot)
        {
            slot = Sprite::createWithSpriteFrameName(slotFileName);
            slot->setAnchorPoint(Vec2(0, 0));
            slot->setPositionX(m_slotDec * s);
            m_slotContener->addChild(slot, 0);
            m_slotList[s] = slot;
        }
        else
        {
            slot->setSpriteFrame(slotFileName);
        }
        if(slotName != "NULL")
        {
            slot->setVisible(true);
        }
        else
        {
            slot->setVisible(false);
            positionX += m_slotDec/2;
        }
        m_slotList[s]->setName(slotName);
    }
    m_slotContener->setPositionX(positionX);
}

//-------------------------------CHESS BOARD---------------------------
void CardDisplay::setChessBoard()
{
    std::vector<std::vector<int>> board = m_specCard->getBoard();
    
    std::string checkBoardFileName = "UI/card/chess_board.png";
    if(!m_chessBoardSprite)
    {
        m_chessBoardSprite = Sprite::createWithSpriteFrameName(checkBoardFileName);
        m_chessBoardSprite->setPosition(m_boardOrigin);
        m_chessBoardSprite->setCascadeOpacityEnabled(true);
        this->addChild(m_chessBoardSprite, 2);
        
    }
    
    auto children = m_chessBoardSprite->getChildren();
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
            std::string boardMoveFileName = "UI/card/board_" + std::to_string(boardSpec[y]) + ".png";
            auto boardMove = Sprite::createWithSpriteFrameName(boardMoveFileName);
            boardMove->setAnchorPoint(Vec2(0.5, 0.5));
            boardMove->setPosition(Vec2(originX, originY));
            boardMove->setColor(boardColor);
            m_chessBoardSprite->addChild(boardMove);
        }
        for(int w = 3; w < boardSpec.size(); w++)
        {
            std::string overBoardMoveFileName = "UI/card/over_board_" + std::to_string(boardSpec[w]) + ".png";
            auto overboardMove = Sprite::createWithSpriteFrameName(overBoardMoveFileName);
            overboardMove->setAnchorPoint(Vec2(0.5, 0.5));
            overboardMove->setPosition(Vec2(originX, originY));
            m_chessBoardSprite->addChild(overboardMove, 1);
        }
    }
    if(board.size() > 0){
        std::string yellowDotFileName = "UI/card/board_0.png";
        auto yellowDot = Sprite::createWithSpriteFrameName(yellowDotFileName);
        yellowDot->setAnchorPoint(Vec2(0.5, 0.5));
        yellowDot->setPosition(m_boardMoveOrigin);
        m_chessBoardSprite->addChild(yellowDot, 2);
        
        std::string overYellowDotFileName = "UI/card/over_board_0.png";
        auto overYellowDot = Sprite::createWithSpriteFrameName(overYellowDotFileName);
        overYellowDot->setAnchorPoint(Vec2(0.5, 0.5));
        overYellowDot->setPosition(m_boardMoveOrigin);
        overYellowDot->setColor(boardMoveColor[board[0][2]]);
        m_chessBoardSprite->addChild(overYellowDot, 2);
    }
}

//-------------------------------LEFT CARD---------------------------
void CardDisplay::setLeft()
{
    //create and place left point conteneur
    if(!m_leftPointConteneur)
    {
        m_leftPointConteneur = Node::create();
        m_leftPointConteneur->setAnchorPoint(Vec2(0.5, 0.5));
        m_leftPointConteneur->setPosition(Vec2(0,m_leftPointY));
        m_leftPointConteneur->setCascadeOpacityEnabled(true);
        this->addChild(m_leftPointConteneur, 3);
    }
    
    //get card info
    CardsLeft cardLeft= getCardIsUsed();
    int nbrCardLeft = cardLeft.nbrCardLeft;
    int nbrCardTotal = cardLeft.nbrCardTotal;
    
    if(nbrCardLeft > 1)
    {
        std::string cardLeftFileName = "UI/card/card_left_background.png";
        if(!m_leftCard)
        {
            m_leftCard = Sprite::createWithSpriteFrameName(cardLeftFileName);
            this->addChild(m_leftCard, 0);
        }
        else
        {
            m_leftCard->setSpriteFrame(cardLeftFileName);
        }
        m_leftCard->setVisible(true);
    }
    else
    {
        if(m_leftCard)
        {
            m_leftCard->setVisible(false);
        }
    }
    
    for(int p = 0; p < m_nbrLeftPoint; p++)
    {
        if(nbrCardLeft < nbrCardTotal)
        {
            std::string leftPointFileName = "UI/card/used_card_red.png";
            if(!cardLeft.usedByChar[p])
            {
                leftPointFileName = "UI/card/used_card_void.png";
            }
            
            if(!m_LeftPointList[p])
            {
                m_LeftPointList[p] = Sprite::createWithSpriteFrameName(leftPointFileName);
                m_LeftPointList[p]->setPositionX(m_leftPointDec * p - m_leftPointDec * 2);
                m_leftPointConteneur->addChild(m_LeftPointList[p], 0);
            }
            else
            {
                m_LeftPointList[p]->setSpriteFrame(leftPointFileName);
            }
            m_LeftPointList[p]->setVisible(true);
        }
        else if(m_LeftPointList[p])
        {
            m_LeftPointList[p]->setVisible(false);
        }
    }
    
    if(nbrCardLeft > 0 || m_isSelect == true)
    {
        setImage();
        m_leftPointConteneur->setScale(0.8);
    }
    else
    {
        m_imageSprite->setSpriteFrame("card/" + m_breed + "/_backCard.png");
        m_leftPointConteneur->setScale(1.5);
    }
}

CardsLeft CardDisplay::getCardIsUsed()
{
    CardsLeft cardleft;
    
    auto stuffList = MainStuff::getStuffList();
    
    int typeInt = getIndex(CARD_TYPE, m_type);
    int breedInt = getIndex(BREED_TYPE, m_breed);
    int objectInt = 0;
    if(m_type != "breed" && m_type != "job")
    {
        getIndex(BREED_TYPE, m_object);
    }
    
    cardleft.nbrCardTotal = MainUser::getUserCards()[typeInt][breedInt][objectInt];
    int nbrCardLeft = cardleft.nbrCardTotal;
    
    for(int c = 0; c < 5 ; c++)
    {
        std::string breed = stuffList[c][typeInt][1];
        std::string object = stuffList[c][typeInt][2];
        
        bool isCardUsed = false;
        if(m_breed == breed && m_object == object)
        {
            isCardUsed = true;
            nbrCardLeft --;
        }
        cardleft.usedByChar[c] = isCardUsed;
    }
    
    cardleft.nbrCardLeft = nbrCardLeft;
    
    return cardleft;
}

//-------------------------------SELECT UNSELECT---------------------------
void CardDisplay::setSelect(std::string board)
{
    m_isSelect = true;
    
    //set selected color
    this->setColor(m_selectColor);
    
    //set selected size
    float scale = m_scaleArray.find(board)->second[1];
    this->setScale(scale);
    
    //set left points and left cards
    if(m_board == "library")
    {
        setLeft();
    }
}
void CardDisplay::setUnselect(std::string board)
{
    m_isSelect = false;
    
    //set unselected color
    cocos2d::Color3B color = m_unselectColor.find(board)->second;
    this->setColor(color);
    
    //set unselected size
    float scale = m_scaleArray.find(board)->second[0];
    this->setScale(scale);
    
    //set left points and left cards
    if(m_board == "library")
    {
        setLeft();
    }
}

//-------------------------------FLIP CHESS BOARD---------------------------
void CardDisplay::flipChessBoard(bool isFlipped)
{
    int intIsFlip = (-1 + isFlipped * 2)* -1;
    if(m_chessBoardSprite){
        m_chessBoardSprite->setScaleX(intIsFlip);
        m_chessBoardSprite->setScaleY(intIsFlip);
    }
}

//-------------------------------POPUP--------------------------------------
void CardDisplay::popUp(PopObject popObject)
{
    cocos2d::Node* layer;
    CallFunc* callFunc;
    
    switch (popObject) {
        case IMAGE:
            layer = m_imageLayer;
            callFunc = CallFunc::create([=](){setImage();});
            break;
        case MANA:
            layer = m_manaLayer;
            callFunc = CallFunc::create([=](){setMana();});
            break;
        case SLOTS:
            layer = m_slotsLayer;
            callFunc = CallFunc::create([=](){setSlots();});
            break;
        case BOARD:
            layer = m_chessBoardLayer;
            callFunc = CallFunc::create([=](){setChessBoard();});
            break;
        default:
            break;
    }
    
    float popUpTime = 0.2;
    auto scaleUp = ScaleTo::create(popUpTime, 0);
    auto scaleDown = ScaleTo::create(popUpTime, 1);
    auto scaleUpEase = EaseOut::create(scaleUp, 0.5);
    auto scaleDownEase = EaseIn::create(scaleDown, 0.5);
    
    if(layer && callFunc)
    {
        auto popUpSeq = Sequence::create(scaleUpEase, callFunc, scaleDownEase, NULL);
        layer->runAction(popUpSeq);
    }
}

//###########################################################################
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
    {
        getOrigin = true;
    }
    int charNbr = GameCharacters::getCharSelect()->getNumber();
    int cardCost = MainStuff::getCardSpec(m_type, m_breed, m_object, charNbr)->getMana(getOrigin);
    
    if(charCrystal >= cardCost && charCrystal > charCrystalRed)
    {
        cardAvailable = true;
    }
    
    return cardAvailable;
}

bool CardDisplay::isCardChanged()
{
    int manaOrigin = m_specCard->getMana(true);
    int mana = m_specCard->getMana(false);
    
    if(manaOrigin != mana)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::string CardDisplay::getMetaName()
{
    std::string metaName = m_type + m_breed + m_object;
    
    return metaName;
}

int CardDisplay::getIndex(std::vector<std::string> v, std::string s)
{
    auto it = find(v.begin(), v.end(), s);
 
    if (it != v.end())
    {
        int index = int(it - v.begin());
        return index;
    }
    else
    {
        return -1;
    }
}
