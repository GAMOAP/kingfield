//
//  BoxMaskDisplay.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 15/04/2019.
//
#include "MainGrid.hpp"
#include "MainObject.hpp"

#include "BoxMask.hpp"

#include <iostream>

USING_NS_CC;

BoxMask* BoxMask::createMask(int line)
{
    BoxMask* ret = new (std::nothrow) BoxMask();
    if(ret && ret->init (line))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
    return ret;
}

bool BoxMask::init(int line)
{
    auto mainGrid = MainGrid::getInstance();
    
    m_line = line;
    m_index = mainGrid->getPositionXYZ(line, 2).z + 3;
    m_position.x = mainGrid->getPositionXYZ(line, 2).x;
    m_position.y = mainGrid->getPositionXYZ(line, 2).y;
    
    this->setTag(-1);
    this->setPosition(m_position);
    this->setLocalZOrder(m_index);
    
    auto mainScene = MainObject::getMainLayer();
    mainScene->addChild(this, m_index);
    
    auto mask = Sprite::createWithSpriteFrameName("screen/boxes_mask.png");
    mask->setAnchorPoint(Vec2( 0.5, 1));
    mask->setPosition(Vec2(0, -80));
    this->addChild(mask);
    
    return true;
}
