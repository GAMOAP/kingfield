//
//  KFSpecCard.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 27/11/2019.
//

#ifndef KFSpecCard_hpp
#define KFSpecCard_hpp

#include "cocos2d.h"

#include "json/document.h"

class KFSpecCard
{
public:
    static KFSpecCard* createCard(std::string type, std::string breed, std::string object = "");
    
    std::string getType();
    std::string getBreed();
    std::string getObject();
    
    int getMana(bool isOrigin);
    
    std::vector<std::string> getSlotList();
    std::string getSlot(int number);
    std::string getSlot(std::string slotName);
    
    std::vector<std::vector<int>> getBoard();
    
    enum BuffName{mana};
    bool setCardBuff(BuffName name, int value);
    
    bool setCardLevel(int value);
    int getCardLevel();
    
private:
    bool init(std::string type, std::string breed, std::string object);
    
    void setKFcardSpec();
    
    void setMana(rapidjson::Value& spec);
    void setBoard(rapidjson::Value& spec);
    void setSlotList(rapidjson::Value& spec);
    
    
protected:
    std::string m_type;
    std::string m_breed;
    std::string m_object;
    
    int m_mana;
    int m_manaOrigin;
    
    int m_level;
    
    std::vector<std::string> m_slotList;
    
    std::vector<std::vector<int>> m_board;
};

static rapidjson::Document m_KFcardSpec = nullptr;


#endif /* KFSpecCard_hpp */
