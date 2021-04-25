//
//  GameCards.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 12/07/2019.
//

#include "MainObject.hpp"
#include "MainStuff.hpp"

#include "GameBoxes.hpp"
#include "GameCharacters.hpp"
#include "GameCards.hpp"

#include "DeckCard.hpp"
#include "SheetCard.hpp"
#include "LibraryCard.hpp"

//#include <algorithm>
#include <iostream>


USING_NS_CC;

GameCards* GameCards::getInstance()
{
    if (!m_SharedGameCards)
    {
        m_SharedGameCards = new (std::nothrow) GameCards;
        m_SharedGameCards->init();
    }
    return m_SharedGameCards;
}
bool GameCards::init()
{
    return true;
}

// deck.
void GameCards::addDeck(){ m_SharedGameCards->add("deck");}
void GameCards::removeDeck(){ m_SharedGameCards->remove("deck");}

// sheet.
void GameCards::addSheet(){ m_SharedGameCards->add("sheet");}
void GameCards::removeSheet(){ m_SharedGameCards->remove("sheet");}

// library.
void GameCards::addLibrary(){m_SharedGameCards->add("library");}
void GameCards::removeLibrary(){ m_SharedGameCards->remove("library");}

void GameCards::removeAll()
{
    m_SharedGameCards->remove("deck");
    m_SharedGameCards->remove("sheet");
    m_SharedGameCards->remove("library");
}

void GameCards::add(std::string board)
{
    for(int c = 0; c < 9; c++)
    {
        if(board == "deck" && c < 4)
            DeckCard::setCard(c, board);
        else if(board == "sheet" && c > 3)
            SheetCard::setCard(c, board);
        else if(board == "library")
            LibraryCard::setCard(c, board);
    }
}

void GameCards::remove(std::string board)
{
    auto nodeList = MainObject::getMainLayer()->getChildren();
    Vector<Node*>::iterator nlIt;
    
    for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
    {
        auto card = dynamic_cast<Card*>(*nlIt);
        if(card && card->getBoard() == board)
        {
            card->removeToStage();
            GameBoxes::setBoxUnselect(card->getTag());
        }
    }
    auto cardSelected = m_SharedGameCards->m_cardSelected;
    if(cardSelected && cardSelected->getBoard() == board)
    {
        m_SharedGameCards->m_cardSelected = nullptr;
    }
}

//---------------------CARD SELECT------------------------------
//board and deck--------
void GameCards::setCardSelect(int number, std::string board)
{
    auto nodeList = MainObject::getMainLayer()->getChildren();
    std::vector<Node*>::iterator nlIt;
    for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
    {
        auto card = dynamic_cast<Card*>(*nlIt);
        if(card)
        {
            if(card->getNumber() == number && card->getBoard() == board)
            {
                card->setSelect();
                m_SharedGameCards->m_cardSelected = card;
                m_SharedGameCards->resetLibrary();
            }
            else if(card->getBoard() != "library")
            {
                card->setUnselect();
            }
        }
    }
}
void GameCards::setCardsChange()
{
    auto nodeList = MainObject::getMainLayer()->getChildren();
    std::vector<Node*>::iterator nlIt;
    for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
    {
        auto card = dynamic_cast<DeckCard*>(*nlIt);
        if(card)
        {
            card->setTexture(true);
        }
    }
}
//library-------------
void GameCards::setCardSelectLibrary(Card* cardCliqued)
{
    const auto charSelect = GameCharacters::getCharSelect();
    const auto charKing = GameCharacters::getKingFriend();
    const auto charUI = GameCharacters::getCharUI();
    const auto cardSelect = getCardSelect();
    
    std::string cardCliquedTBO = cardCliqued->getType_Breed_object();
    std::string cardSelectTBO = cardSelect->getType_Breed_object();
    
    if(charSelect && cardSelect && cardCliqued && cardCliqued->isVisible() && cardCliquedTBO != cardSelectTBO)
    {
        const int charNumber = charSelect->getNumber();
        const std::string type = cardCliqued->getType();
        const std::string breed = cardCliqued->getBreed();
        const std::string object = cardCliqued->getObject();
        MainStuff::setStuff(charNumber, type, breed, object);
        charSelect->setStuff();
        charKing->setFlag();
        charUI->setCharacterUI(charNumber);
        cardSelect->initDisplay();
        cardCliqued->setSelect();
        
        auto nodeList = MainObject::getMainLayer()->getChildren();
        std::vector<Node*>::iterator nlIt;
        for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
        {
            auto card = dynamic_cast<LibraryCard*>(*nlIt);
            if(card && card->getNumber() != cardCliqued->getNumber())
            {
                card->setUnselect();
                card->place(0);
                setCardsChange();
            }
        }
    }
}
//Unselect-----------------
void GameCards::unselectAll()
{
    if(m_SharedGameCards->m_cardSelected)
    {
        m_SharedGameCards->m_cardNumberMemory = m_SharedGameCards->m_cardSelected->getNumber();
        
        m_SharedGameCards->m_cardSelected->setUnselect();
        m_SharedGameCards->m_cardSelected = nullptr;
    }
    
    if(m_SharedGameCards->m_cardSelectedLibrary)
    {
        m_SharedGameCards->m_cardSelectedLibrary->setUnselect();
        m_SharedGameCards->m_cardSelectedLibrary = nullptr;
    }
}
//----------------------------MEMORY---------------------------
int GameCards::getCardNumberMemory()
{
    return m_SharedGameCards->m_cardNumberMemory;
}

//Cards Reset.
void GameCards::resetCards()
{
    auto nodeList = MainObject::getMainLayer()->getChildren();
    std::vector<Node*>::iterator nlIt;
    for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
    {
        auto card = dynamic_cast<Card*>(*nlIt);
        if(card)
        {
            if(card->getBoard() != "library")
            {
                card->initDisplay();
            }
            else if(m_SharedGameCards->m_cardSelected)
            {
                card->setUnselect();
                card->place(0);
            }
        }
    }
}

void GameCards::resetLibrary()
{
    auto nodeList = MainObject::getMainLayer()->getChildren();
    std::vector<Node*>::iterator nlIt;
    for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
    {
        auto card = dynamic_cast<LibraryCard*>(*nlIt);
        if(card)
        {
            if(card->isEqual(m_SharedGameCards->m_cardSelected))
                card->setSelect();
            
            card->initDisplay();
        }
    }
}

bool GameCards::setLibraryPage(int pagePlus)
{
    bool lastPage = true;
    m_SharedGameCards->m_libraryPage = 0;
        
    int cardSelectedNbr = m_SharedGameCards->m_cardSelected->getNumber();
    std::string cardSelectedBoard = m_SharedGameCards->m_cardSelected->getBoard();
    
    if((lastPage && pagePlus > 0) || (m_SharedGameCards->m_libraryPage <= 0 && pagePlus < 0))
    {
        if(getBoardByCardNumber(cardSelectedNbr) == "sheet")
        {
            if(cardSelectedNbr == 4 && pagePlus > 0)
                pagePlus = -4;
            else
                pagePlus *= -1;
        }
        else if(getBoardByCardNumber(cardSelectedNbr) == "deck" && cardSelectedNbr == 3 && pagePlus > 0)
            pagePlus = 5;
            
        int nbr = cardSelectedNbr + pagePlus;
        
        
        if(nbr > 8)nbr = 3;
        if(nbr < 0)nbr = 4;
        GameCards::setCardSelect(nbr, getBoardByCardNumber(nbr));
        
        return true;
    }
    else
    {
        //if there are more of one page.
        
        return false;
    }
}

void GameCards::CardsReseted()
{
    m_SharedGameCards->m_cardSelected->setSelect();
    m_SharedGameCards->resetLibrary();
}

//Function Get.
Card* GameCards::getCardSelect()
{
    return m_SharedGameCards->m_cardSelected;
}

Card* GameCards::getCardByTag(int tag)
{
    auto nodeList = MainObject::getMainLayer()->getChildren();
    std::vector<Node*>::iterator nlIt;
    for(nlIt = nodeList.begin(); nlIt != nodeList.end(); nlIt++)
    {
        auto card = dynamic_cast<Card*>(*nlIt);
        if(card && card->getTag() == tag)
        {
            return card;
        }
    }
    return nullptr;
}
std::string GameCards::getBoardByCardNumber(int number)
{
    std::string board;
    if(number < 4)
        board = "deck";
    else
        board = "sheet";
    
    return board;
}
