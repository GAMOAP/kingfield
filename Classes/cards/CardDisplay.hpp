//
//  CardDisplay.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 12/07/2019.
//

#ifndef CardDisplay_hpp
#define CardDisplay_hpp

#include "cocos2d.h"

struct CardsLeft
{
    unsigned int nbrCardLeft;
    int usedByChar[5] = {false,false,false,false,false};
};

class CardDisplay : public cocos2d::Node
{
public:
    static CardDisplay* create(std::string type, std::string breed, std::string object, std::string board);
    
    bool setTexture(std::string type, std::string breed, std::string object, std::string board);
    bool setTexture(std::string type, std::string breed, std::string object, bool popUpActived = false);
    bool setChange();
    
    void setSelect(std::string board);
    void setUnselect(std::string board);
    
    void flipChessBoard(bool isFlipped);
    
    std::string getMetaName();
    
private:
    void setDisplay();
    
    void setImage();
    void setMana();
    void setSlots();
    void setChessBoard();
    void setLeft();
    
    bool isCardAvailable();
    CardsLeft getCardIsUsed();
    bool isCardChanged();
    
    int getIndex(std::vector<std::string> v, std::string s);
    
protected:
    std::string m_type;
    std::string m_breed;
    std::string m_object;
    std::string m_board;
    
    bool m_popUpActived;
    
    bool m_isSelect;
    
    cocos2d::Sprite* m_image = nullptr;
    cocos2d::Sprite* m_mana = nullptr;
    cocos2d::Sprite* m_manaNbr = nullptr;
    cocos2d::Sprite* m_chessBoard = nullptr;
    
    cocos2d::Sprite* m_leftCard = nullptr;
    cocos2d::Node* m_leftPointConteneur = nullptr;
    const int m_nbrLeftPoint = 5;
    cocos2d::Sprite* m_LeftPointList[5];
    
    cocos2d::Node* m_slotContener = nullptr;
    const int m_nbrSlot = 3;
    cocos2d::Sprite* m_slotList[3];
    
    cocos2d::Vec2 m_manaPosition = {-32, 48 };
    cocos2d::Vec2 m_manaNbrPosition = {24, 28};
    
    const int m_slotStartX = -56;
    const int m_slotStartY = -72;
    const int m_slotDec = 40;
    
    const int m_leftPointX = 56;
    const int m_leftPointY = -80 ;
    const int m_leftPointDec = 12;
    
    const cocos2d::Vec2 m_boardOrigin = {32, 48};
    const cocos2d::Vec2 m_boardMoveOrigin = {40, 40};
    const int m_boardDec = 16;
    const std::vector<cocos2d::Color3B> boardMoveColor= {
        {0, 0, 255},
        {255, 0, 0},
        {0, 255, 0}};
   
    const cocos2d::Color3B m_selectColor = {255, 255, 255};
    const std::map<std::string, cocos2d::Color3B> m_unselectColor= {
        {"deck", {230, 220, 230}},
        {"library", {200, 200, 240}},
        {"sheet", {220, 210, 210}}};
    
    const std::map<std::string, std::vector<float>> m_scaleArray= {
        {"deck", {0.9, 1.1, 1.2}},
        {"library", {0.7, 0.9, 1.2}},
        {"sheet", {0.8, 1, 1.2}}};
};

#endif /* CardDisplay_hpp */
