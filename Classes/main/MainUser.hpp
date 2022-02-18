//
//  MainUser.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 06/09/2019.
//

#ifndef MainUser_hpp
#define MainUser_hpp

class MainUser
{
public:
    static void reset();
    
    static std::string getUserName();
    
    static bool setUserTeam(std::vector<std::vector<std::vector<std::string>>> userTeamList);
    static std::vector<std::vector<std::vector<std::string>>> getUserTeam();
    
    static std::vector<std::vector<std::vector<int>>> getUserCards();
    
    static bool setOpposingTeam(std::string opposingTeamStr);
    static std::string getUserTeamStr();
    
private:
    static std::vector<std::vector<std::vector<std::string>>> getTeamVec(std::string teamStr);
    static std::vector<std::vector<std::vector<int>>> getCardsVec(std::string cardsStr);
    static std::vector<std::string> splitString(std::string mString, std::string mChar);
    static int getBreedInt(std::string breed);
    
};

#endif /* MainUser_hpp */
