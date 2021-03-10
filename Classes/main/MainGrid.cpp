//
//  MainGrid.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 28/04/2019.
//

#include "MainGrid.hpp"

#include <iostream>

USING_NS_CC;

MainGrid* MainGrid::getInstance()
{
    if (!m_SharedMainGrid)
    {
        m_SharedMainGrid = new (std::nothrow) MainGrid;
        m_SharedMainGrid->init();
    }
    return m_SharedMainGrid;
}

bool MainGrid::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    m_visibleWidth = visibleSize.width;
    m_visibleHeight = visibleSize.height;
      
    m_leftToOriginX = (m_visibleWidth - m_realResolutionSizeWidth) / 2;
    m_downToOriginY = (m_visibleHeight - m_resolutionSizeHeight) / 2;
    
    m_ratioCorrectionY = m_visibleHeight / 2 - m_resolutionSizeHeight / 2;
    if(m_ratioCorrectionY > m_caseHeight / 2 && m_ratioCorrectionY < m_caseHeight)
    {
        m_ratioCorrectionY = m_caseHeight / 2;
    }
    else if(m_ratioCorrectionY > m_caseHeight)
    {
        m_ratioCorrectionY -= m_caseHeight;
    }
    
    m_originX = m_visibleWidth / 2 - m_centerScreenToOriginCenterCaseWidth;
    m_originY = m_visibleHeight / 2 - (m_centerScreenToOriginCenterCaseHeigth + m_ratioCorrectionY);
    
    m_leftToOriginX = m_originX - m_caseWidth / 2;
    m_downToOriginY = m_originY - m_caseHeight / 2;
    
    for(int l = 0; l < m_nbrLine; l++){
        for (int c = 0; c < m_nbrCollumn; c++) {
            
            m_grid[l][c][0] = m_caseWidth * (c + m_originCollumn) + m_originX;
            m_grid[l][c][1] = m_caseHeight * (l + m_originLine) + m_originY;
            m_grid[l][c][2] = m_nbrLine * 3 - (l + 1) * 3;
            m_grid[l][c][3] = (l * 10 + c);
            
            int randomInt = random(0, 40);
            int randomTime = randomInt + l;
            m_grid[l][c][4] = randomTime;
        }
    }
    return true;
}

int MainGrid::getPosition(int line, int collumn, int xyz){
    int coord(0);
    
    int l = line - m_originLine;
    int c = collumn  - m_originCollumn;
    if(&m_grid[l][c][xyz])
    coord = m_grid[l][c][xyz];
    
    return coord;
}

// Get Position.
Vec3 MainGrid::getPositionXYZ(int line, int collumn)
{
    Vec3 coord;
    coord.set(getPosition(line, collumn, 0),getPosition(line, collumn, 1),getPosition(line, collumn, 2));
    return coord;
}

// Get Ramdom.
float MainGrid::getRandom(int tag)
{
    int l = tag / 10;
    int c = tag - l * 10;
    int line = l + m_originLine;
    int collumn = c + m_originCollumn;
    
    float random = getPosition(line, collumn, 4);
    float coord = random / m_randomDivisor;
    
    return coord;
}
int MainGrid::getMaxRandomTag(std::vector<int> tagList)
{
    int randomMaxTag = 0;
    for(int t = 0; t < tagList.size(); t++)
    {
        if(getRandom(tagList[t]) > getRandom(randomMaxTag))
        {
            randomMaxTag = tagList[t];
        }
    }
    return randomMaxTag;
}

// Get Tag.
int MainGrid::getTag(Vec2 lc)
{
    int tag = getPosition(lc.x, lc.y, 3);
    return tag;
}
int MainGrid::getTagByLocation(Vec2 xy)
{
    int coord = getTag(Vec2(getLineCollumnByLocation(xy).x, getLineCollumnByLocation(xy).y));
    
    return coord;
}

// Get Line Collumn.
cocos2d::Vec2 MainGrid::getLineCollumnByLocation(Vec2 xy)
{
    float x = xy.x - m_leftToOriginX;
    float y = xy.y - m_downToOriginY;
    int line = std::floor(y / m_SharedMainGrid->m_caseHeight);
    int collumn = std::floor(x / m_SharedMainGrid->m_caseWidth);
    
    cocos2d::Vec2 xyVector;
    xyVector.x = line;
    xyVector.y = collumn;
    
    return xyVector;
}
cocos2d::Vec2 MainGrid::getLineCollumnByTag(int tag)
{
    cocos2d::Vec2 xyVector;
    for(int l = 0; l < m_nbrLine; l++){
        for (int c = 0; c < m_nbrCollumn; c++) {
            
            if(m_grid[l][c][3] == tag)
            {
                xyVector.x = l + m_originLine;
                xyVector.y = c + m_originCollumn;
            }
        }
    }
    return xyVector;
}

int MainGrid::getLineNumber(){ return m_nbrLine + m_originLine;}
int MainGrid::getCollumnNumber(){ return m_nbrCollumn + m_originCollumn;}
int MainGrid::getOriginLine(){ return m_originLine;}
int MainGrid::getOriginCollumn(){ return m_originCollumn;}
cocos2d::Vec2 MainGrid::getVisibleSize()
{
    return Vec2(m_SharedMainGrid->m_visibleWidth, m_SharedMainGrid->m_visibleHeight);
}
