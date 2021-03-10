//
//  KFSprite.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 26/09/2019.
//

#ifndef KFSprite_hpp
#define KFSprite_hpp

#include "cocos2d.h"

class KFSprite : public cocos2d::Sprite
{
public:
    static std::string getFile(const std::string& filename);
    
private:
    static std::string getScaleStr();
    
};

#endif /* KFSprite_hpp */
