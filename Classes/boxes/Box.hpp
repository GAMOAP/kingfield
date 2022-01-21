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

#include "BoxDisplay.hpp"

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
    
    void setActionUI(int actionType, int becameTag, bool touchAuth);
    void resetActionUI();
    bool getIsActionUI();
    
    std::string getBreed();
    std::string getType();
    std::string getLastType();
    
private:
    virtual bool init(int line, int collumn);
    void initDisplay();
    
    void removeBox();
    
    void rumble();
    
protected:
    std::string m_scene;
    std::string m_lastType;
    std::string m_type;
    std::string m_breed;
    std::string m_secondBreed;
    
    bool m_isRumble;
    
    BoxDisplay* m_boxDisplay = nullptr;
    cocos2d::Sprite* m_boxActionUI = nullptr;
    cocos2d::Sprite* m_boxPathActionUI = nullptr;
    bool m_actionUI;
    bool m_touchAuth;
    
    //BarackTag.
    const std::vector<int> m_barrackFieldTag = {10,11,16,20,21,22,23,24,25,26,30,36,40,46,50,56,60,66,70,71,72,73,74,75,76};
    const std::vector<int> m_barrackBarrackTag = {12,13,14,15,31,32,33,34,35,41,42,43,44,45,51,52,53,54,55,31,61,62,63,64,65};
    
    //FightTag;
    const std::vector<int> m_fightFieldTag = {10,11,16,20,21,22,23,24,25,26,30,31,32,33,34,35,36,40,41,42,43,44,45,46,50,51,52,53,54,55,56,60,61,62,63,64,65,66,70,71,72,73,74,75,76};
    const std::vector<int> m_fightBarrackTag = {12,13,14,15};
    
    //Remove sheduled
    const std::vector<int> m_libraryTag = {32,33,34,42,43,44,52,53,54,62,63,64};
    
    const cocos2d::Color3B m_colorSelect = {255, 255, 255};
    const cocos2d::Color3B m_colorUnselect = {210, 210, 210};
};
#endif /* Box_hpp */
