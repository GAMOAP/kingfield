//
//  KFAction.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 21/11/2019.
//

#include "KFAction.hpp"

KFAction* KFAction::setAction(int type, int charNbr)
{
    KFAction* ret = new (std::nothrow) KFAction();
    if(ret && ret->init(type, charNbr))
    {
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}
bool KFAction::init(int type, int charNbr)
{
    m_type = type;
    m_charNbr = charNbr;
    
    return true;
}
KFAction::~KFAction()
{
    
}

int KFAction::getType()
{
    return m_type;
}
int KFAction::getCharNbr()
{
    return m_charNbr;
}

//------------------CRYSTAL COST---------------------
void KFAction::setCost(int crystalCost)
{
    m_crystalCost = crystalCost;
}
int KFAction::getCost()
{
    return m_crystalCost;
}

//------------------SLOT LIST---------------------
void KFAction::setSlotSpecNbr(cocos2d::Vec3 slotListNbr)
{
    m_slotListNbr = slotListNbr;
}
cocos2d::Vec3 KFAction::getSlotListNbr()
{
    return m_slotListNbr;
}

//Move
void KFAction::setStartTag(int startTag){ m_startTag = startTag;}
int KFAction::getSartTag(){ return m_startTag;}

void KFAction::setEndTag(int endTag){ m_endTag = endTag;}
int KFAction::getEndTag(){ return m_endTag;}

//Strike


//Striked

