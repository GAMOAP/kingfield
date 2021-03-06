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

enum m_xpState{start, end, manage};

class Character : public KFNode
{
public:
    
    static Character* setCharacter(int number);
    
    virtual bool removeToStage();
    
    void setStuff(std::string type);
    
    bool setAction(KFAction* action);
    
    void setSelect(bool isTurn = true);
    void setUnselect();
    
    void setBuff(std::string buffName);
    void manageBuffs(bool reset = false);
    std::string getBuffName();
    int getBuffTurnLeft();
    
    bool manageTired(bool reset = false);
    
    bool manageXp(m_xpState state);
    
    void setLevel(int levelNbr);
    int getLevel();
    int getCardLevel(int cardTypeNbr);
    int getCardLevel(std::string cardName);
    
    void setFlag();
    
    void setInfo(std::string infoName, int infoValue = -1);
    
    bool getIsPlayerTeam();
    
    bool isLevelUp();
    bool isSleeping();
    bool isBlocking();
    bool isDead();
    
    void setIfActiveThisTurn(bool isActive);
    bool getIfActiveThisTurn();
    
private:
    
    enum m_reaction{fail, block, pain, death, heal, crystal_break, defense_more, attack_more, defense_less, attack_less, poison, sleep, blocking};
    
    virtual bool init(int number);
    bool initPosition(int number);
    
    void setActionSequence();
    
    bool setMove(int startTag, int endTag);
    bool setStrike(std::vector<std::vector<int>> strikedList, int actionCard, std::string actionSlotType, int force);
    bool setSpell(std::vector<std::vector<int>> bewitchedList, int actionCard, std::string actionSlotType, int force);
    
    bool setEndAction(Character* reactChar, m_reaction reaction, int reactedSize, int reactedNbr);
    
    bool applyBuff(std::string buffName);
     
    std::string setReaction(m_reaction reaction, int actionCharNbr);
    
protected:
    bool m_isPlayerTeam;
    
    bool m_isLevelUp;
    
    bool m_isDead;
    
    CharacterDisplay* m_characterDisplay = nullptr;
    
    const float m_moveTime = 0.6;
    const float m_moveDecTime = 0.2;
    
    const std::vector<int> m_originTagList= {21, 22, 23, 24, 25, 65, 64, 63, 62, 61};
    
    int m_xp;
    int m_level;
    int m_deckCardLevel[4] = {0, 0, 0, 0};
    
    struct
    {
        std::string name = "";
        int startTurn = 0;
        int endTurn = 0;
    }m_buffList;
    
    bool m_isActiveThisTurn;
};
#endif /* Character_hpp */
