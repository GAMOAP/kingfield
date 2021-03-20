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
    
    bool addToInfoList(std::string infoName, int infoValue);
    
private:
    void displayUp();
    void displayPop();
    
    cocos2d::Node* createUpInfo(std::vector<int> info);
    cocos2d::Node* createPopInfo(std::vector<int> info);
    
    cocos2d::Node* m_upDisplay = nullptr;
    cocos2d::Node* m_popDisplay = nullptr;
    
    std::vector<std::vector<int>> m_upList;
    std::vector<std::vector<int>> m_popList;
    
    std::vector<std::string> m_typeNameUpList = {"crystal", "life"};
    std::vector<std::string> m_typeNamePopList = {"defence", "attack"};
};

#endif /* CharacterInfo_hpp */
