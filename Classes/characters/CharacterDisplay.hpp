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

class CharacterDisplay : public cocos2d::Node
{
public:
    static CharacterDisplay* create(int number);
    virtual bool init(int number);
    
    bool setStuff();
    
    bool setFlag();
    
    void setSelect();
    void setUnselect();
    
    void setAction(std::string actionType);
    
    int getOriginTag();
    int getNumber();
    
    void stopAnimation();
    
    void setInfo(std::string infoName, int infoValue);
    
private:
    
    void initCharacter(int number);
    bool createFactory();
    
    void setStuffList();
    void setStuffDisplay();
    
    void setAnimation(std::string animation, int nbrLoop = 0);
    void setState(std::string state);
    void playAnimation();
    
    void displayInfo();
    void createInfo();
    
    std::string getSlotFileDisplay(std::string& slotName);
    
protected:
    
    dragonBones::CCFactory* m_factory;
    dragonBones::CCArmatureDisplay* m_armatureDisplay;
    
    const double m_scaleArray[4] = {1.1, 1.2, 1.3, 1.4};
    const cocos2d::Color3B m_colorSelect = {255, 255, 255};
    const cocos2d::Color3B m_colorUnselect = {200, 200, 210};
    
    const int m_index = 2;
    
    int m_originTag;
    int m_number;
    int m_team;
    
    std::map<std::string, std::vector<std::string>> m_stuffList;
    
    cocos2d::Sprite* m_flag = nullptr;
    std::string m_karmaFlag;
    cocos2d::Vec2 m_flagPos = {20, 68};
    
    bool m_selected = false;
    std::vector<std::string> m_oldAnimationVector = {"stand", "ok"};
    std::vector<std::string> m_animationVector = {"stand", "ok"};
    int m_animationLoopNumber = 0;
    
    cocos2d::Node* m_infoDisplay = nullptr;
    std::vector<std::vector<int>> m_infoList;
    std::vector<std::string> m_typeNameInfoList = {"crystal"};
    
    const std::vector<int> m_originTagList= {21, 31, 41, 51, 61, 25, 35, 45, 55, 65};
};

#endif /* CharacterDisplay_hpp */
