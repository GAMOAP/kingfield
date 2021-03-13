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
    
    void setAction(KFAction* action);
    
    void setSelect();
    void setUnselect();
    
    void setFlag();
    
    void setInfo(std::string infoName, int infoValue);
    
    bool getIsPlayerTeam();
    
private:
    virtual bool init(int number);
    void initDisplay();
    
    void setActionSequence();
    
protected:
    bool m_isPlayerTeam;
    
    CharacterDisplay* m_characterDisplay = nullptr;
    
    const float m_actionTime = 0.5;
    
    //const std::vector<int> m_originTagList= {21, 31, 41, 51, 61, 25, 35, 45, 55, 65};
    const std::vector<int> m_originTagList= {21, 22, 23, 24, 25, 61, 62, 63, 64, 65};
};
#endif /* Character_hpp */
