//
//  Character.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//

#ifndef Character_hpp
#define Character_hpp

#include "cocos2d.h"

#include "KFNode.hpp"
#include "KFAction.hpp"

#include "CharacterDisplay.hpp"


class Character : public KFNode
{
public:
    
    static Character* setCharacter(int number);
    
    virtual bool removeToStage();
    
    void setStuff();
    
    bool setAction(KFAction* action);
    
    void setSelect(bool isTurn = true);
    void setUnselect();
    
    void setBuff(std::string buffName);
    void manageBuffs();
    
    void setFlag();
    
    void setInfo(std::string infoName, int infoValue);
    
    bool getIsPlayerTeam();
    
    bool isSleeping();
    
private:
    virtual bool init(int number);
    void initDisplay();
    
    void setActionSequence();
    
    bool setMove(int endTag);
    bool setStrike(std::vector<std::vector<int>> strikedList, std::string actionSlotType, int force);
    bool setSpell(std::vector<std::vector<int>> bewitchedList, std::string actionSlotType, int force);
    
    bool applyBuff(std::string buffName);
    
    enum m_reaction{fail, block, pain, death, heal, crystal_break, defense_more, attack_more, defense_less, attack_less, poison, sleep, blocking};
    std::string setReaction(m_reaction reaction);
     
protected:
    bool m_isPlayerTeam;
    
    CharacterDisplay* m_characterDisplay = nullptr;
    
    const float m_actionTime = 0.5;
    
    const std::vector<int> m_originTagList= {21, 22, 23, 24, 25, 65, 64, 63, 62, 61};
    
    struct
    {
        std::string name = "";
        int startTurn = 0;
        int endTurn = 0;
    }m_buffList;
};
#endif /* Character_hpp */
