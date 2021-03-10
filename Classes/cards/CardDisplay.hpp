//
//  CardDisplay.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 12/07/2019.
//

#ifndef CardDisplay_hpp
#define CardDisplay_hpp

#include "cocos2d.h"

class CardDisplay : public cocos2d::Node
{
public:
    static CardDisplay* create(std::string type, std::string breed, std::string object, std::string board);
    
    bool setTexture(std::string type, std::string breed, std::string object, bool popUpActived = false);
    bool setChange();
    
    void setSelect();
    void setUnselect();
    
    void flipChessBoard(bool isFlipped);
    
    std::string getMetaName();
    
private:
    void setDisplay();
    
    void setImage();
    void setMana();
    void setSlots();
    void setChessBoard();
    
    bool isCardAvailable();
    bool isCardChanged();
    
protected:
    std::string m_type;
    std::string m_breed;
    std::string m_object;
    std::string m_board;
    
    bool m_popUpActived;
    
    cocos2d::Sprite* m_image = nullptr;
    cocos2d::Sprite* m_mana = nullptr;
    cocos2d::Sprite* m_manaNbr = nullptr;
    cocos2d::Sprite* m_chessBoard = nullptr;
    
    cocos2d::Node* m_slotContener = nullptr;
    const int m_nbrSlot = 3;
    cocos2d::Sprite* m_slotList[3];
    
    cocos2d::Vec2 m_manaPosition = {-32, 48 };
    cocos2d::Vec2 m_manaNbrPosition = {24, 28};
    
    const int slotStartX = -56;
    const int slotStartY = -72;
    const int slotDec = 40;
    
    const cocos2d::Vec2 m_boardOrigin = {32, 48};
    const cocos2d::Vec2 m_boardMoveOrigin = {40, 40};
    const int m_boardDec = 16;
    const std::vector<cocos2d::Color3B> boardMoveColor= {
        {0, 0, 255},
        {255, 0, 0},
        {0, 255, 0}};
};

#endif /* CardDisplay_hpp */
