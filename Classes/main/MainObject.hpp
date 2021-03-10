//
//  MainObject.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 30/05/2019.
//

#ifndef MainObject_hpp
#define MainObject_hpp

#include "KFNode.hpp"

#include "Box.hpp"
#include "Character.hpp"
#include "CharacterUI.hpp"
#include "Card.hpp"

class  MainObject
{
public:
    
    static MainObject* getInstance();
    bool init();
    
    static void setMainLayer(cocos2d::Node* mainLayer);
    static cocos2d::Node* getMainLayer();
    static void setInfoLayer(cocos2d::Node* infoLayer);
    static cocos2d::Node* getInfoLayer();
    
    //Node
    static bool isAllNodeIsIn();
    static bool isNodesMove();
    static KFNode* getNodeByTag(int tag, bool noBox = false);
    static int getNumberNodeByTag(int tag);
    
    //Boxes
    static Box* getBoxByLC(int line, int collumn);
    static Box* getBoxByTag(int tag);
    
    //Character
    static Character* getCharByNumber(int number);
    static Character* getCharByTag(int tag);
    
    //ChararcterUI
    static CharacterUI* getCharacterUI();
    
    //Card
    static Card* getCardByNumber(int number, std::string board);
    static Card* getCardByTag(int card);
    
private:
    
    
protected:
    cocos2d::Node* m_mainLayer;
    cocos2d::Node* m_infoLayer;
};

static MainObject* m_SharedMainObject = nullptr;


    
#endif /* MainObject_hpp */
