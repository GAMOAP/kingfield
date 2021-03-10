//
//  CharacterUI.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//

#ifndef CharacterUI_hpp
#define CharacterUI_hpp

#include "cocos2d.h"

#include "Character.hpp"

class CharacterUI : public cocos2d::Node
{
public:
    static CharacterUI* setCharacterUI(int charNumber);
    static void removeCharacterUI();
    
    static CharacterUI* getOptionCharacterUI(int charNumber);
    
private:
    void createCrystal();
    void createLife();
    void createKarma();
    void createDefense();
    void createAttack();
    void createUnderlight();
    
    void addToStage();
    void addToInfo();
    
    void popUp(cocos2d::Sprite* sprite, std::string name, bool isVisible = true);
    
protected:
    bool init(int charNumber);
    bool initStage(int charNumber);
    bool initOption(int charNumber);
    
    int m_charNumber;
    std::map<std::string, int> m_charSpec;
    std::string m_karmaBreed;
    
    const int m_indexUnderlight = 2;
    const int m_indexUI = 2;
    
    //Crystal
    int m_crystalNbr = 12;
    std::vector<cocos2d::Sprite*> m_crystalList;
    const int m_crystalListX = -55;
    const int m_crystalListY = 99;
    
    //life
    int m_lifeNbr = 12;
    std::vector<cocos2d::Sprite*> m_lifeList;
    const int m_lifeListX = -28;
    const int m_lifeListY = 84;
    
    //Karma
    cocos2d::Sprite* m_karma = nullptr;
    const cocos2d::Vec2 m_karmaPosition = {-50, 74};
    
    //Defense
    cocos2d::Sprite* m_defense = nullptr;
    const cocos2d::Vec2 m_defensePosition = {-40, -38};
    
    //attack
    cocos2d::Sprite* m_attack = nullptr;
    const cocos2d::Vec2 m_attackPosition = {40, -38};
    
    //Underlight
    cocos2d::Sprite* m_underlight = nullptr;
    
    //popUp
    bool m_popUpActived;
};

static CharacterUI* m_SharedCharacterUI = nullptr;
static CharacterUI* m_SharedOptionCharacterUI = nullptr;

#endif /* CharacterUI_hpp */
