//
//  BoxFieldDisplay.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 10/01/2022.
//

#ifndef BoxFieldDisplay_hpp
#define BoxFieldDisplay_hpp

#include "cocos2d.h"

class BoxFieldDisplay : public cocos2d::Sprite
{
public:
    static BoxFieldDisplay* create(int line, int collumn, std::string breed, std::string secondBreed);
    
    void setTexture(int line, int collumn, std::string breed, std::string secondBreed);
    
    void setBoxColor(const cocos2d::Color3B color);
    
    ~BoxFieldDisplay();
    
private:
    virtual bool init(int line, int collumn, std::string breed, std::string secondBreed);
    
    void setBreedArray(int line);
    
    
protected:
    
    bool m_breedArray[6];
    
    bool m_bitArray2[6][6] = {{0,0,0,1,0,0}, {0,0,0,0,1,0}, {0,0,0,0,0,1}, {0,0,0,1,1,0}, {0,0,0,1,0,1}, {0,0,0,0,1,1}};
    
    bool m_bitArray3[17][6] = {{1,1,1,0,0,0}, {1,1,0,1,0,0}, {1,1,0,0,1,0}, {1,1,0,0,0,1}, {1,0,1,1,0,0}, {1,0,1,0,1,0}, {1,0,1,0,0,1}, {1,0,0,1,1,0}, {1,0,0,1,0,1}, {1,0,0,0,1,1}, {0,1,1,1,0,0}, {0,1,1,0,1,0}, {0,1,1,0,0,1}, {0,0,1,1,1,0}, {0,0,1,1,0,1}, {0,0,1,0,1,1}, {0,0,0,1,1,1}};
    
    cocos2d::Sprite* m_miniBoxArray[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    cocos2d::Sprite* m_backBox = nullptr;
    cocos2d::Sprite* m_effectBox = nullptr;
    cocos2d::Sprite* m_maskBox = nullptr;
    cocos2d::Sprite* m_gridBox = nullptr;
};


#endif /* BoxFieldDisplay_hpp */
