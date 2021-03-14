//
//  MainAction.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 23/08/2019.
//

#ifndef MainAction_hpp
#define MainAction_hpp

#include "KFAction.hpp"

class MainAction
{
public:
    static std::vector<cocos2d::Vec3> getActionBoxesTags(int charSelectNbr, int cardSelectNbr);
    
    static std::vector<KFAction*> getActionSequence(int charSelectNbr, int cardSelectNbr, int touchedBoxTag);
    static std::vector<KFAction*> getEnemyActionSequence(int charSelectNbr, int cardSelectNbr, int touchedBoxTag);
    
private:
    static bool isInBoardTag(int tag);
    static bool isCharTag(int tag);
    
    static std::vector<int> setForbiddenTagList(int charTag, std::vector<std::vector<int>> board);
    static bool isForbiddenTag(int tag, std::vector<int> forbiddenTagList);
    
    static int getStartTag(int charNbr, int boardActionC, int boardActionL);
    static std::vector<int> getBoardMap(int charNbr);
    
protected:
    
};

const std::vector<int> m_friendBoardMap = {0, 10, 11, 1, -9, -10, -11, -1, 9};
const std::vector<int> m_enemyBoardMap = {0, -10, -11, -1, 9, 10, 11, 1, -9};

const std::vector<int> m_boardIn = {21,22,23,24,25,31,32,33,34,35,41,42,43,44,45,51,52,53,54,55,61,62,63,64,65};


#endif /* MainAction_hpp */
