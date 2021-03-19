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
    
    //SLOT SPEC
    void setSlotSpecNbr(cocos2d::Vec3 slotListNbr);
    cocos2d::Vec3 getSlotListNbr();
    
    //move
    void setStartTag(int startTag);
    int getSartTag();
    
    void setEndTag(int endTag);
    int getEndTag();
    
    //strike
    
    
    //spell
    
    
private:
    bool init(int type, int charNbr);
    
protected:
    int m_type; //0 = move, 1 = strike, 2 = spell.
    int m_charNbr;
    
    int m_crystalCost;
    
    cocos2d::Vec3 m_slotListNbr;
    
    //move
    int m_startTag;
    int m_endTag;
    
    //stike
    
    
    //spell
    
};

#endif /* KFAction_hpp */
