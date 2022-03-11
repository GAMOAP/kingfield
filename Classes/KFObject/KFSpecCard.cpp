//
//  KFSpecCard.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 27/11/2019.
//

#include "KFSpecCard.hpp"

#include "MainStuff.hpp"

USING_NS_CC;

KFSpecCard* KFSpecCard::createCard(std::string type, std::string breed, std::string object)
{
    KFSpecCard* ret = new (std::nothrow) KFSpecCard();
    if(ret && ret->init(type, breed, object))
    {
        return ret;
    }
    else
    {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool KFSpecCard::init(std::string type, std::string breed, std::string object)
{
    setKFcardSpec();
    
    m_type = type;
    m_breed = breed;
    m_object = object;
    m_level = 0;
    
    if(!m_KFcardSpec.IsObject())
    {
        std::printf("m_card_spec_isn't_object\n");
        return false;
    }
    else
    {
        setSlotList(m_KFcardSpec[m_type.c_str()][m_breed.c_str()]);
        
        if(m_object != "")
        {
            setMana(m_KFcardSpec[m_type.c_str()][m_breed.c_str()][m_object.c_str()]);
            setBoard(m_KFcardSpec[m_type.c_str()][m_breed.c_str()][m_object.c_str()]);
        }
    }
    
    return true;
}

void KFSpecCard::setKFcardSpec()
{
    if(!m_KFcardSpec.IsObject())
    {
        rapidjson::Document cardSpec;
        std::string filePath = "res/json/card_spec.json";
        std::string prevPath = FileUtils::getInstance()->fullPathForFilename(filePath);
        const auto data = cocos2d::FileUtils::getInstance()->getStringFromFile(prevPath);
        if (data.empty())
        {
            std::printf("card_spec_data_empy");
        }
        else
        {
            m_KFcardSpec.Parse(data.c_str());
        }
    }
}

bool KFSpecCard::setCardBuff(BuffName buffName, int value)
{
    switch (buffName) {
        case mana:
            m_mana = m_manaOrigin + value;
            break;
            
        default:
            break;
    }
    return true;
}

bool KFSpecCard::setCardLevel(int value)
{
    m_level = value;
    
    std::string levelStr = "level" + std::to_string(m_level);
    if(m_object != "" && m_KFcardSpec[m_type.c_str()][m_breed.c_str()][m_object.c_str()].HasMember(levelStr.c_str()))
    {
        
    }
    
    return true;
}

std::string KFSpecCard::getType()
{
    return m_type;
}
std::string KFSpecCard::getBreed()
{
    return m_breed;
}
std::string KFSpecCard::getObject()
{
    return m_object;
}

//Mana-------------------------------
void KFSpecCard::setMana(rapidjson::Value& JsonValue)
{
    if(m_object != "" && JsonValue.HasMember("mana"))
    {
        const rapidjson::Value& spec = JsonValue["mana"];
        m_manaOrigin = cocos2d::Value(spec.GetInt()).asInt();
    }
    m_mana = m_manaOrigin;
}
int KFSpecCard::getMana(bool isOrigin)
{
    if(isOrigin)
        return m_manaOrigin;
    else
        return m_mana;
}

//Board-----------------------------------
void KFSpecCard::setBoard(rapidjson::Value& spec)
{
    if(spec.HasMember("board"))
    {
        const rapidjson::Value& specBoard = spec["board"];
        for( rapidjson::SizeType i = 0; i < specBoard.Size(); i++)
        {
            std::vector<int> vect;
            const rapidjson::Value& specItem = specBoard[i];
            for( rapidjson::SizeType y = 0; y < specItem.Size(); y++)
            {
                const rapidjson::Value& spec = specItem[y];
                int value = cocos2d::Value(spec.GetInt()).asInt();
                vect.push_back(value);
            }
            m_board.push_back(vect);
        }
    }
}

//SlotList----------------------------
void KFSpecCard::setSlotList(rapidjson::Value& spec)
{
    for (int s = 0; s < 3; s++)
    {
        std::string slot = "slot" + std::to_string(s);
        if(m_object != "")
        {
            const rapidjson::Value& value = spec[m_object.c_str()][slot.c_str()];
            m_slotList.push_back(cocos2d::Value(value.GetString()).asString());
        }
        else
        {
            const rapidjson::Value& value = spec[slot.c_str()];
            m_slotList.push_back(cocos2d::Value(value.GetString()).asString());
        }
    }
}

std::vector<std::string> KFSpecCard::getSlotList()
{
    return m_slotList;
}
std::string KFSpecCard::getSlot(int number)
{
    return m_slotList[number];
}
std::string KFSpecCard::getSlot(std::string slotName)
{
    slotName.erase(0, 4);
    int number = std::stoi(slotName);
    return m_slotList[number];
}

std::vector<std::vector<int>> KFSpecCard::getBoard()
{
    return m_board;
}
