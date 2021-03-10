//
//  MainGrid.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 28/04/2019.
//


#ifndef MainGrid_hpp
#define MainGrid_hpp

#include "cocos2d.h"

class  MainGrid
{
public:
    static MainGrid* getInstance();
    bool init();
    
    static cocos2d::Vec3 getPositionXYZ(int line, int collumn);
    
    static int getTag(cocos2d::Vec2 lc);
    static int getTagByLocation(cocos2d::Vec2 xy);
    
    static float getRandom(int tag);
    static int getMaxRandomTag(std::vector<int> tagList);
    
    static cocos2d::Vec2 getLineCollumnByLocation(cocos2d::Vec2 xy);
    static cocos2d::Vec2 getLineCollumnByTag(int tag);
    
    static int getCollumnNumber();
    static int getOriginCollumn();
    static int getLineNumber();
    static int getOriginLine();
    static cocos2d::Vec2 getVisibleSize();
    
private:
    static int getPosition(int line, int collumn, int xy);
    
    int m_visibleWidth;
    int m_visibleHeight;
    static const int m_originLine = -2;
    static const int m_originCollumn = -1;
    static const int m_caseWidth = 128;
    static const int m_caseHeight = 160;
    static const int m_centerScreenToOriginCenterCaseWidth = 256;
    static const int m_centerScreenToOriginCenterCaseHeigth = 272;
    static const int m_resolutionSizeHeight = 1024;
    static const int m_realResolutionSizeWidth = 640;
    static const int m_nbrLine = 11;
    static const int m_nbrCollumn = 7;
    static const int m_randomDivisor = 100;
};

static MainGrid* m_SharedMainGrid = nullptr;

static int m_originX;
static int m_originY;
static int m_ratioCorrectionY;//m_grid[m_nbrLine][m_nbrCollumn][CreateGrid-Attribute];
static int m_leftToOriginX;
static int m_downToOriginY;
static int m_grid[11][7][5];

#endif /* MainGrid_hpp */
