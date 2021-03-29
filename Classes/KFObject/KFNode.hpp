//
//  KFNode.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 23/09/2019.
//

#ifndef KFNode_hpp
#define KFNode_hpp

#include "cocos2d.h"


/**
@brief    KingField Objects.
 Box, Characters, Cards.

ineriteance of cocos node
*/
class KFNode : public cocos2d::Node
{
public:
    
    /**
    @brief    get the class of KFNode Object: Unitilised!
    */
    virtual KFNode* getClassObject();
    
    //------------------------------POSITION---------------------------
    
    /**
    @brief    set m_index, m_position, this position, Zorder, Tag and Metaname
        use MainGrid getPositionXYZ.
    @return true    Initialize success, app continue.
    */
    virtual bool setNodePosition();
    
    /**
    @brief    set m_isFliped to -1
    @return true    Initialize success, app continue.
    */
    virtual void setFlip();
    
    //--------------------------------ADD REMOVE TO STAGE-----------------
    /**
    @brief    add or remove object: use move in and move out
    @return true success, app continue / false Print "NO CLASS NAME"
    */
    virtual bool addToStage();
    
    /**
    @brief    add or remove object: use move in and move out
    @return true success, app continue / false Print "NO CLASS NAME"
    */
    virtual bool removeToStage();
    
    void add(float speedFactor = 1, float delayFactor = 1);
    void remove(float speedFactor = 1, float delayFactor = 1, std::string movement = "both");
    
    void up(float speedFactor = 1, bool isCascadedMove = true);
    void place(float speedFactor = 1, bool isCascadedMove = true);
    void down(float speedFactor = 1, bool isCascadedMove = true);
    
    bool getIsIn();
    bool getIsOut();
    
    void setIsMove(bool isMove);
    bool getIsMove();
    
    bool getIsPlace();
    
    bool getIsSelect();
    
    std::string getCurrentAction();
    
    std::map<std::string, int> getPositionLCI();
    
    std::string getClassName();
    std::string getMetaName();
    
    void setNumber(int number);
    int getNumber();
    
    cocos2d::Node* getDisplayNode();
    
    bool getIsActionUI();
    
private:
    void setMetaName(std::string metaName);
    
    void runActionName(std::string action, float speedFactor = 1, float delayFactor = 1, std::string movement = "both");
    void runActionSeq(std::string action, cocos2d::Sequence* seq, float speedFactor = 1, float delayFactor = 1, std::string movement = "both");
    void finishAction(std::string action);
    
protected:
    
    int getIndexClassName();
    
    cocos2d::Size m_visibleSize;
    
    std::string m_className;
    std::string m_metaName;
    
    int m_number;
    
    int m_line;
    int m_collumn;
    int m_index;
    cocos2d::Vec2 m_position;
    
    bool m_isIn = true;
    bool m_isOut = false;
    bool m_isMove = false;
    bool m_isPlace = true;
    bool m_select = false;
    int m_isFlipped = 1;
    
    //m_cascadedMove : block library box up,place and down when a card is selected.
    bool m_cascadedMove = true;
    
    const int m_centerHeigth = -240;
    const int m_moveInOutRange = 320 ;
    
    const int m_moveRange = 8;
    const float m_moveTime = 0.2 ;
    
    std::string m_currentAction = "";
    cocos2d::Sequence* m_runningSeq = nullptr;
    
};

#endif /* KFNode_hpp */
