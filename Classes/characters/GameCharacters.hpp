//
//  GameCharacters.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//

#ifndef GameCharacters_hpp
#define GameCharacters_hpp

#include "cocos2d.h"

#include "KFAction.hpp"

#include "Character.hpp"
#include "CharacterUI.hpp"

class GameCharacters
{
public:
    static GameCharacters* getInstance();
    
    static void setCharacters(int charNbr);
    static void removeAllCharacters();
    
    //character select
    static void setCharSelect(int number = -1);
    static Character* getCharSelect();
    static bool getCharIsSelected();
    static void unselectAll();
    
    //king
    static Character* getKingFriend();
    
    //memory
    static int getCharNumberMemory();
    
    //action
    static void setAction(std::vector<KFAction*> actionSequence);
    static void setActionAll(std::string actionName);
    
    //character UI
    static CharacterUI* getCharUI();
    
private:
    bool init();
    
    void setActionSequence(Character* character);
        
protected:
    Character* m_charSelected = nullptr;
    CharacterUI* m_characterUI = nullptr;
    
    Character* m_kingFriend = nullptr;
    Character* m_kingEnemy = nullptr;
    
    std::vector<KFAction*> m_actionSequence;
    int m_sequenceState;
    
    int m_charNumberMemory = -1;
};

static GameCharacters* m_SharedGameCharacters = nullptr;

#endif /* GameCharacters_hpp */
