//
//  DeckCard.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 17/07/2019.
//

#ifndef DeckCard_hpp
#define DeckCard_hpp

#include "cocos2d.h"

#include "Card.hpp"
#include "CardDisplay.hpp"

class DeckCard : public Card
{
public:
    static DeckCard* setCard(int number, std::string board);
    
    virtual bool init(int number);
    
    virtual bool initDisplay();
    
private:
    
    
protected:
    
};

#endif /* DeckCard_hpp */
