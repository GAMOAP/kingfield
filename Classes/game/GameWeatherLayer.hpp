//
//  GameWeatherLayer.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 21/01/2022.
//

#ifndef GameWeatherLayer_hpp
#define GameWeatherLayer_hpp

#include "cocos2d.h"

class GameWeatherLayer
{
public:
    static GameWeatherLayer* getInstance();
    
    static void addClouds();
    static void removeClouds();
    
private:
    virtual bool init();
    
    bool setCloud();
    
protected:
    cocos2d::Node* m_weatherLayer = nullptr;
    
    cocos2d::Node* m_cloudLayer = nullptr;
    
};
    
static GameWeatherLayer* m_SharedGameWeatherLayer= nullptr;


#endif /* GameWeatherLayer_hpp */
