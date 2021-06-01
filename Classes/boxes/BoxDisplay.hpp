//
//  BoxDisplay.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/05/2021.
//

#ifndef BoxDisplay_hpp
#define BoxDisplay_hpp

#include "cocos2d.h"

class BoxDisplay : public cocos2d::Node
{
public:
    static BoxDisplay* create(int line, int collumn, std::string type, std::string breed, std::string secondBreed);
    
    void setTexture(int line, int collumn, std::string type, std::string breed, std::string secondBreed);
    
    void setColor(const cocos2d::Color3B color);
    
    
private:
    virtual bool init(int line, int collumn, std::string type, std::string breed, std::string secondBreed = "");
    
    
protected:
    
    cocos2d::Sprite* m_backBox = nullptr;
    cocos2d::Sprite* m_frontBox = nullptr;
   
};

#endif /* BoxDisplay_hpp */
