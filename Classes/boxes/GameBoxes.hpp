//
//  GameBoxes.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 04/07/2019.
//

#ifndef GameBoxes_hpp
#define GameBoxes_hpp

#include "cocos2d.h"

#include "Box.hpp"

class GameBoxes
{
public:
    static GameBoxes* getInstance();
    static void setBoxes();
    
    static void setBoxSelect(int tag);
    static void setBoxUnselect(int tag);
    
    static void setFightButtonBox(bool isAuth);
    static bool getFightButtonBoxAuth();
    
    static bool startRumbleBox(std::vector<int> boxesTagList);
    static bool stopRumbleBox();
    
    static void setSelectActionBoxes(std::vector<cocos2d::Vec3> selectedActionBoxes);
    static void setUnselectActionBoxes();
    
    static void unselectAll();
    
private:
    bool init();
    
protected:
    bool m_fightButtonBoxAuth;
};

static GameBoxes* m_SharedGameBoxes = nullptr;

#endif /* GameBoxes_hpp */
