//
//  KFSpecCard.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 27/11/2019.
//

#include "KFSpecCard.hpp"

#include "MainStuff.hpp"

USING_NS_CC;

KFSpecCard* KFSpecCard::createCard(std::string type, std::string breed, std::string object, int level)
{
    KFSpecCard* ret = new (std::nothrow) KFSpecCard();
    if(ret && ret->init(type, breed, object, level))
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

bool KFSpecCard::init(std::string type, std::string breed, std::string object, int level)
{
    setKFcardSpec();
    
    m_type = type;
    m_breed = breed;
    m_object = object;
    m_level = level;
    
    if(!m_KFcardSpec.IsObject())
    {
        std::printf("m_card_spec_isn't_object\n");
        return false;
    }
    else
    {
        if(m_object != "")
        {
            m_mana = m_manaOrigin = setMana(m_KFcardSpec[m_type.c_str()][m_breed.c_str()][m_object.c_str()]);
            m_board = setBoard(m_KFcardSpec[m_type.c_str()][m_breed.c_str()][m_object.c_str()]);
            m_slotList = setSlotList(m_KFcardSpec[m_type.c_str()][m_breed.c_str()][m_object.c_str()]);
        }
        else
        {
            m_slotList = setSlotList(m_KFcardSpec[m_type.c_str()][m_breed.c_str()]);
        }
    }
    
    if(m_level > 0 && (m_type == "move" || m_type == "spell" || m_type == "weapon" || m_type == "object"))
    {
        std::string levelStr = "level" + std::to_string(m_level);
        rapidjson::Value& levelSpec = m_KFcardSpec[m_type.c_str()][m_breed.c_str()][m_object.c_str()];
        
        if(levelSpec.HasMember(levelStr.c_str()))
        {
            rapidjson::Value& spec = m_KFcardSpec[m_type.c_str()][m_breed.c_str()][m_object.c_str()][levelStr.c_str()];
            if(spec.HasMember("mana"))
            {
                m_manaOrigin = setMana(spec);
            }
            if(spec.HasMember("board"))
            {
                m_board = setBoard(spec);
            }
            for (int s = 0; s < 3; s++)
            {
                std::string slot = "slot" + std::to_string(s);
                if(spec.HasMember(slot.c_str()))
                {
                    m_slotList = setSlotList(spec);
                }
            }
        }
        printf("\n");
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
int KFSpecCard::getLevel()
{
    return m_level;
}

//Mana-------------------------------
int KFSpecCard::setMana(rapidjson::Value& JsonValue)
{
    int mana = 0;
    
    if(m_object != "" && JsonValue.HasMember("mana"))
    {
        const rapidjson::Value& spec = JsonValue["mana"];
        mana = cocos2d::Value(spec.GetInt()).asInt();
    }
    
    return mana;
}
int KFSpecCard::getMana(bool isOrigin)
{
    if(isOrigin)
        return m_manaOrigin;
    else
        return m_mana;
}

//Board-----------------------------------
std::vector<std::vector<int>> KFSpecCard::setBoard(rapidjson::Value& spec)
{
    std::vector<std::vector<int>> board;
    
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
            board.push_back(vect);
        }
    }
    
    return board;
}

std::vector<std::vector<int>> KFSpecCard::getBoard()
{
    return m_board;
}

//SlotList----------------------------
std::vector<std::string> KFSpecCard::setSlotList(rapidjson::Value& spec)
{
    std::vector<std::string> slotList;
    
    for (int s = 0; s < 3; s++)
    {
        std::string slot = "slot" + std::to_string(s);
        const rapidjson::Value& value = spec[slot.c_str()];
        slotList.push_back(cocos2d::Value(value.GetString()).asString());
    }
    
    return slotList;
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
