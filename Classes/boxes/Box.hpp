//
//  Box.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 03/06/2019.
//

#ifndef Box_hpp
#define Box_hpp

#include "cocos2d.h"

#include "KFNode.hpp"
#include "KFSprite.hpp"

class Box : public KFNode
{
public:
    virtual Box* getClassObject();
    
    static Box* setBox(int line, int collumn);
    
    void setSelect(float speedFactor = 1);
    void setUnselect();
    
    void startRumble();
    void stopRumble();
    bool getIsRumble();
    
    void setActionUI(int actionType,bool touchAuth);
    void resetActionUI();
    bool getIsActionUI();
    
private:
    virtual bool init(int line, int collumn);
    void initDisplay();
    
    void removeBox();
    
    void rumble();
    
protected:
    std::string m_scene;
    std::string m_type;
    std::string m_breed;
    std::string m_oldFile;
    
    std::string m_boxFile;
    bool m_flipped;
    
    bool m_isRumble;
    
    cocos2d::Sprite* m_boxDisplay = nullptr;
    cocos2d::Sprite* m_boxActionUI = nullptr;
    bool m_actionUI;
    bool m_touchAuth;
    
    //BarackTag.
    const std::vector<int> m_barrackFieldTag = {10,11,20,21,30,31,40,41,50,51,60,61,70,71,72,73};
    const std::vector<int> m_barrackBarrackTag = {0,1,2,3,4,5,6,12,13,14,15,22,23,24,25,32,33,34,35,
        42,43,44,45,52,53,54,55,62,63,64,65,80,81,82,83,84,85,86,90,91,92,93,93,94,95,96};
    const std::vector<int> m_barrackBarrackTagInvert = {16,26,36,46,56,66,75,76,74};
    
    //FightTag;
    const std::vector<int> m_fightFieldTag = {10,11,20,21,22,23,24,30,31,32,33,34,40,41,42,43,44,50,51,52,53,54,60,61,62,63,64,70,71,72,73};
    const std::vector<int> m_fightBarrackTag = {0,1,2,3,4,5,6,12,13,14,15,80,81,82,83,84,85,86,90,91,92,93,93,94,95,96};
    const std::vector<int> m_fightFieldTagInvert = {16,24,25,26,34,35,36,44,45,46,54,55,56,64,65,66,74,75,76};
    
    //Remove sheduled
    const std::vector<int> m_libraryTag = {32,33,34,42,43,44,52,53,54,62,63,64};
    
    const cocos2d::Color3B m_colorSelect = {255, 255, 255};
    const cocos2d::Color3B m_colorUnselect = {220, 220, 220};
};
#endif /* Box_hpp */
