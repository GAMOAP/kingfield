//
//  BoxMaskDisplay.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 15/04/2019.
//

#ifndef BoxMask_hpp
#define BoxMask_hpp

#include "cocos2d.h"


class BoxMask : public cocos2d::Node
{
public:
    static BoxMask* createMask(int line);
    virtual bool init(int line);

private:
    int m_line;
    int m_index;
    cocos2d::Vec2 m_position;
};

#endif /* BoxMask_hpp */
