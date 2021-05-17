//
//  GameCharacters.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//

#ifndef GameCharacters_hpp
#define GameCharacters_hpp

#include "cocos2d.h"

#include "Constants.h"

#include "KFAction.hpp"

#include "Character.hpp"
#include "CharacterUI.hpp"

class GameCharacters
{
public:
    static GameCharacters* getInstance();
    
    static void setCharacters(int charNumbers);
    static void removeAllCharacters();
    static void removeCharacter(int charNbr);
    
    //character select
    static void setCharSelect(int number = -1);
    static Character* getCharSelect();
    static bool getCharIsSelected();
    static void unselectAll(bool eraseMemory = false);
    
    //king
    static Character* getKingFriend();
    
    //memory
    static int getCharNumberMemory();
    static int getCharTagMemory(int charNbr);
    
    //action
    static void setAction(std::vector<KFAction*> actionSequence);
    static void setActionAll(std::string actionName);
    static bool getIsActionRun();
    
    //character UI
    static CharacterUI* getCharUI();
    
private:
    bool init();
    
    bool initChar(int charNbr);
    
    void setActionSequence(Character* character);
    void endActionSequence(Character* character);
        
protected:
    Character* m_charSelected = nullptr;
    CharacterUI* m_characterUI = nullptr;
    
    Character* m_kingFriend = nullptr;
    Character* m_kingEnemy = nullptr;
    
    std::vector<KFAction*> m_actionSequence;
    int m_sequenceState;
    bool m_isActionRun = false;
    
    int m_charNumberMemory = -1;
    
    int m_charactersTagsMemory[CHAR_NUMBER];
    const int m_charactersTagsMemoryOrigin[CHAR_NUMBER] = {21, 22, 23, 24, 25, 65, 64, 63, 62, 61};
};

static GameCharacters* m_SharedGameCharacters = nullptr;

#endif /* GameCharacters_hpp */
