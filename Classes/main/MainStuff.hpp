//
//  MainStuff.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//

#ifndef MainStuff_hpp
#define MainStuff_hpp

#include "Constants.h"

#include "KFSpecCard.hpp"

class  MainStuff
{
public:
    
    static MainStuff* getInstance();
    bool init();
    
    static bool initCharStuffList();
    
    static bool setStuff(int charNumber, std::string type, std::string breed, std::string object);
    
    static std::vector<std::vector<std::vector<std::string>>> getStuffList();
    
    static std::vector<std::string> getStuffByName(int charNumber, int cardNumber);
    
    static std::string getKarma(int charNumber);
    static std::string getTeamKarma(int teamNumber);
    
    //SPEC.
    //Char.
    static void initCharSpec(int charNbr);
    static void setCharSpec(int charNbr, std::string specName, int value);
    static std::map<std::string, int> getCharSpec(int charNbr);
    
    //Card.
    static KFSpecCard* getCardSpec(std::string type, std::string breed, std::string object);
    static void setCardSpecChangeManaCost(std::string type, bool reset = false);
    static void resetCardSpecManaCost();
    
private:
    
    
protected:
    std::map<std::string, int> m_charactersSpec[CHAR_NUMBER];
    
    std::vector<KFSpecCard*>m_cardSpecLibrary;
    
    std::vector<std::vector<std::vector<std::string>>> m_charStuffList;
};

static MainStuff* m_SharedMainStuff = nullptr;



#endif /* MainStuff_hpp */
