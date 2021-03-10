//
//  GameCards.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 12/07/2019.
//

#ifndef GameCards_hpp
#define GameCards_hpp

#include "cocos2d.h"

#include "Card.hpp"

class GameCards
{
public:
    static GameCards* getInstance();
    
    static void addDeck();
    static void removeDeck();
    
    static void addSheet();
    static void removeSheet();
    
    static void addLibrary();
    static void removeLibrary();
    
    static void removeAll();
    
    static void setCardSelect(int number, std::string board);
    static void setCardSelectLibrary(Card* card);
    static void unselectAll();
    static int getCardNumberMemory();
    
    static void setCardsChange();
    
    static bool setLibraryPage(int pagePlus);
    
    static void resetCards();
    static void resetLibrary();
    
    static void CardsReseted();
    
    static Card* getCardSelect();
    static Card* getCardByTag(int tag);
    
    static std::string getBoardByCardNumber(int number);
    
private:
    bool init();
    
    void add(std::string board);
    void remove(std::string board);
    
    bool setCardSelectLibrary(int number);
    
    
protected:
    Card* m_cardSelected = nullptr;
    Card* m_cardSelectedLibrary = nullptr;
    
    int m_cardNumberMemory = NAN;
    
    int m_libraryPage;
};

static GameCards* m_SharedGameCards = nullptr;

#endif /* GameCards_hpp */
