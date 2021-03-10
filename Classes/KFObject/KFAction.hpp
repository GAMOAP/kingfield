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
    
    //move
    void setStartTag(int startTag);
    int getSartTag();
    
    void setEndTag(int startTag);
    int getEndTag();
    
private:
    bool init(int type, int charNbr);
    
protected:
    int m_type; //0 = move, 1 = instant, 2 = nextTurn.
    int m_charNbr;
    
    int m_crystalCost;
    
    //move
    int m_startTag;
    int m_endTag;
};

#endif /* KFAction_hpp */
