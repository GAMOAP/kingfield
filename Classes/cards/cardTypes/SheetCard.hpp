//
//  SheetCard.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 17/07/2019.
//

#ifndef SheetCard_hpp
#define SheetCard_hpp

#include "Card.hpp"

class SheetCard : public Card
{
public:
    static SheetCard* setCard(int number, std::string board);
    
    bool virtual init(int number);
    
    bool virtual initDisplay();
    
private:
    
    
protected:
    
};

#endif /* SheetCard_hpp */
