//
//  BoxDisplay.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/05/2021.
//

#ifndef BoxDisplay_hpp
#define BoxDisplay_hpp

#include "cocos2d.h"

#include "BoxFieldDisplay.hpp"

class BoxDisplay : public cocos2d::Node
{
public:
    static BoxDisplay* create(int line, int collumn, std::string type, std::string breed, std::string secondBreed, std::string scene);
    
    void setTexture(int line, int collumn, std::string type, std::string breed, std::string secondBreed, std::string scene);
    
    void setBoxColor(const cocos2d::Color3B color);
    
    
private:
    virtual bool init(int line, int collumn, std::string type, std::string breed, std::string secondBreed, std::string scene);
    
    cocos2d::Sprite* setDisplayBox(cocos2d::Sprite* box, std::string fileName, int index);
    
    
protected:
    
    int m_line;
    int m_collumn;
    std::string m_type;
    std::string m_breed;
    std::string m_secondBreed;
    
    cocos2d::Sprite* m_backBox = nullptr;
    BoxFieldDisplay* m_fieldBox = nullptr;
   
};

#endif /* BoxDisplay_hpp */
