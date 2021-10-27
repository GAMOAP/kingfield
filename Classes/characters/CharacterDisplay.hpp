//
//  CharacterDisplay.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//

#ifndef CharacterDisplay_hpp
#define CharacterDisplay_hpp

#include "cocos2d.h"
#include "../proj.ios_mac/dragonBones/cocos2dx/CCDragonBonesHeaders.h"

#include "CharacterInfo.hpp"

enum OutlineColor
{
    white,black,blue,red
};

class CharacterDisplay : public cocos2d::Sprite
{
public:
    static CharacterDisplay* create(int number);
    virtual bool init(int number);
    
    bool setStuff(std::string type);
    
    bool setFlag();
    
    void setSelect();
    void setUnselect();
    
    bool setAnimation(std::string animation, int nbrLoop = 0, bool playLastAnimation = true, int linkedCharNbr = -1);
    bool setState(std::string state);
    
    std::string getAnimationName();
    std::string getStateName();
    
    int getOriginTag();
    int getNumber();
    
    void stopAnimation();
    
    void setInfo(std::string infoName, int infoValue);
    
private:
    
    void initCharacter(int number);
    bool createFactory();
    
    void setStuffList();
    void setStuffDisplay();
    
    bool setOutline(OutlineColor outlineColor = black, float lineSize = 1);
    
    bool playAnimation();
    bool animationEnd(cocos2d::Event* event);
    
    bool setExpression(std::string animationName);
    
    void displayInfo();
    void createInfo();
    
    std::string getSlotFileDisplay(std::string& slotName);
    
protected:
    
    dragonBones::CCFactory* m_factory;
    dragonBones::CCArmatureDisplay* m_armatureDisplay;
    
    cocos2d::GLProgram* m_glprogram = nullptr;
    cocos2d::GLProgramState* m_glprogramState = nullptr;
    std::vector<cocos2d::Sprite*> m_outlineSpriteList;
    
    const double m_scaleArray[4] = {1, 1.1, 1.2, 1.3};
    const cocos2d::Color3B m_colorSelect = {255, 255, 255};
    const cocos2d::Color3B m_colorUnselect = {200, 200, 210};
    
    const int m_index = 2;
    
    int m_originTag;
    int m_number;
    int m_team;
    
    bool m_alive = true;
    
    std::map<std::string, std::vector<std::string>> m_stuffList;
    
    cocos2d::Sprite* m_flag = nullptr; 
    cocos2d::Vec2 m_flagPos = {20, 68};
    
    bool m_selected = false;
    std::vector<std::string> m_animationVector = {"stand", "ok"};
    std::string m_lastAnimationName = "";
    int m_animationLoopNumber = 0;
    bool m_playLastAnimation = true;
    int m_linkedCharNbr = -1;
    
    CharacterInfo* m_charInfo = nullptr;
};

#endif /* CharacterDisplay_hpp */
