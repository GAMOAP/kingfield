//
//  KFSprite.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/09/2019.
//

#include "KFSprite.hpp"

USING_NS_CC;

std::string KFSprite::getFile(const std::string& filename)
{
    return filename + getScaleStr();
}

std::string KFSprite::getScaleStr()
{
    std::string scaleStr;
    auto director = Director::getInstance();
    int resolutionSizeWidth = director->getOpenGLView()->getFrameSize().width;
    
    if(resolutionSizeWidth < 1000)
        scaleStr = "@2x.png";
    else
        scaleStr = "@3x.png";
    
    return scaleStr;
}
