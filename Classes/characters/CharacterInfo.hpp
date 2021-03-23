//
//  CharacterInfo.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 19/03/2021.
//

#ifndef CharacterInfo_hpp
#define CharacterInfo_hpp

#include "cocos2d.h"

class CharacterInfo : public cocos2d::Node
{
public:
    static CharacterInfo* create();
    virtual bool init();
    
    bool addToInfoList(std::string infoName, int infoValue, int characterNbr);
    
private:
       
    int m_characterNbr;
    
    void displayUp();
    void displayPop();
    
    cocos2d::Node* createUpInfo(std::vector<int> info);
    cocos2d::Node* createPopInfo(std::vector<int> info);
    
    cocos2d::Node* m_upDisplay = nullptr;
    cocos2d::Node* m_popDisplay = nullptr;
    
    std::vector<std::vector<int>> m_upList;
    std::vector<std::vector<int>> m_popList;
    
    std::vector<std::string> m_typeNameUpList = {"crystal", "health"};
    std::vector<std::string> m_typeNamePopList = {"defense", "attack"};
    
    cocos2d::Sprite* m_pop_picture = nullptr;
    cocos2d::Sprite* m_popValueTotalBackgroud = nullptr;
    cocos2d::Sprite* m_popValue = nullptr;
    cocos2d::Sprite* m_popValueTotalfront = nullptr;
    
    const cocos2d::Color3B m_colorDefense = {60, 36, 21};
    const cocos2d::Vec2 m_defensePosition = {-40, -38};
    
    const cocos2d::Color3B m_colorAttack = {237, 28, 36};
    const cocos2d::Vec2 m_attackPosition = {40, -38};
};

#endif /* CharacterInfo_hpp */
