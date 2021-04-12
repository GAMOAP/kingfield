//
//  Card.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 12/07/2019.
//

#ifndef Card_hpp
#define Card_hpp

#include "cocos2d.h"

#include "KFNode.hpp"
#include "CardDisplay.hpp"

class Card : public KFNode
{
public:
    virtual Card* getClassObject();
    
    virtual bool initPosition();
    
    virtual bool initDisplay() = 0;
    
    virtual void setTexture(bool popUp = false);
    
    virtual void setSelect();
    virtual void setUnselect();
    
    virtual void removeCard();
    
    CardDisplay* getCardDisplay();
    
    virtual std::string getBoard();
    virtual std::string getType();
    virtual std::string getBreed();
    virtual std::string getObject();
    virtual std::string getType_Breed_object();
    virtual int getCost();
    
    virtual bool isEqual(const Card* b)const;
    
private:
    
    
protected:
    std::string m_board = "";
    
    std::string m_type = "";
    std::string m_breed = "";
    std::string m_object = "";
    
    bool m_isEnemyTeam;
    
    CardDisplay* m_cardDisplay = nullptr;
    
    std::vector<int> m_tagList;
    std::vector<float> m_scaleArray;
    cocos2d::Color3B m_colorSelect;
    cocos2d::Color3B m_colorUnselect;
};

#endif /* card_hpp */
