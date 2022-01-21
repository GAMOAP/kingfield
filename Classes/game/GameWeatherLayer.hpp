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
    
private:
    virtual bool init();
    
    bool setCloud(cocos2d::Sprite* cloud);
    
protected:
    cocos2d::Node* m_weatherLayer = nullptr;
    
    cocos2d::Sprite* m_cloud_1 = nullptr;
    cocos2d::Sprite* m_cloud_2 = nullptr;
    
};
    
static GameWeatherLayer* m_SharedGameWeatherLayer= nullptr;


#endif /* GameWeatherLayer_hpp */
