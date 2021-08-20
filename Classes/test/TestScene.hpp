//
//  TestScene.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2021.
//

#ifndef TestScene_hpp
#define TestScene_hpp

#include "cocos2d.h"

class TestScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void moveSprite(cocos2d::Sprite* sprite);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TestScene);
};

#endif /* TestScene_hpp */
