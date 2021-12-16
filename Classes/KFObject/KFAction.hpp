//
//  KFAction.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 21/11/2019.
//

#ifndef KFAction_hpp
#define KFAction_hpp

class KFAction
{
public:
    static KFAction* setAction(int type, int charNbr);
    ~KFAction();
    
    int getType();
    int getCharNbr();
    
    //CRYSTAL COST
    void setCost(int crystalCost);
    int getCost();
    
    //CARD SELECT NBR
    void setCardNbr(int cardSelectNbr);
    int getCardNbr();
    
    //SLOT SPEC
    void setSlotSpec(std::string slotSpec);
    std::string getSlotSpec();
    
    //TURN NUMBER
    void setFightTurn(int turnNbr);
    int getFightTurn();
    
    //move
    void setStartTag(int startTag);
    int getSartTag();
    
    void setEndTag(int endTag);
    int getEndTag();
    
    //strike
    void setCharStrikedList(std::vector<std::vector<int>> strikedList);
    std::vector<std::vector<int>> getCharStrikedList();
    
    void setCharAttackForce(int charForce);
    int getCharAttackForce();
    
    //spell
    
    
private:
    bool init(int type, int charNbr);
    
protected:
    int m_type; //0 = move, 1 = strike, 2 = spell.
    int m_charNbr;
    
    int m_crystalCost;
    
    int m_cardNbr;
    
    std::string m_slotSpec;
    
    int m_turnNbr;
    
    //move
    int m_startTag;
    int m_endTag;
    
    //stike
    std::vector<std::vector<int>> m_strikedList;
    int m_charForce;
    
    //spell
    
};

#endif /* KFAction_hpp */
