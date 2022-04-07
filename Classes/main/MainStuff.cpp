//
//  MainStuff.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 05/07/2019.
//
#include "Constants.h"

#include "MainStuff.hpp"
#include "MainUser.hpp"

#include "MainObject.hpp"

#include "GameDirector.hpp"

#include <json/istreamwrapper.h>
#include <fstream>

#include <iostream>

USING_NS_CC;

MainStuff* MainStuff::getInstance()
{
    if (!m_SharedMainStuff)
    {
        m_SharedMainStuff = new (std::nothrow) MainStuff;
        m_SharedMainStuff->init();
    }
    return m_SharedMainStuff;
}

bool MainStuff::init()
{
    initCharStuffList();
    
    //init char buffLine
    for(int c = 0; c < CHAR_NUMBER; c++)
    {
        m_lineBuff[c] = -1;
    }
    
    std::string filePath = "res/json/card_spec.json";
    std::string prevPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    const auto data = cocos2d::FileUtils::getInstance()->getStringFromFile(prevPath);
        
    for(int t = 0; t < CARD_TYPE.size(); t++)
    {
        for(int b = 0; b < BREED_TYPE.size(); b++)
        {
            KFSpecCard* stuffCard = nullptr;
            for(int cardLevel = 0; cardLevel < CARD_LEVELS_NBR; cardLevel++)
            {
                if(CARD_TYPE[t] == "breed" || CARD_TYPE[t] == "job")
                {
                    stuffCard = KFSpecCard::createCard(CARD_TYPE[t], BREED_TYPE[b], "", cardLevel);
                    m_cardSpecLibrary.push_back(stuffCard);
                }
                else
                {
                    for(int o = 0; o < BREED_TYPE.size(); o++)
                    {
                        stuffCard = KFSpecCard::createCard(CARD_TYPE[t], BREED_TYPE[b], BREED_TYPE[o], cardLevel);
                        m_cardSpecLibrary.push_back(stuffCard);
                    }
                }
            }
        }
    }
    return true;
}
bool MainStuff::initCharStuffList()
{
    m_SharedMainStuff->m_charStuffList = MainUser::getUserTeam();
    
    return true;
}

//------------------------------STUFF--------------------------------------------
std::vector<std::vector<std::vector<std::string>>> MainStuff::getStuffList()
{
    return m_SharedMainStuff->m_charStuffList;
}

bool MainStuff::setStuff(int charNumber, std::string typeString, std::string breed, std::string object)
{
    int typeInt = -1;
    for(int t = 0; t < CARD_TYPE.size(); t++)
    {
        std::string ts = CARD_TYPE[t];
        if(ts == typeString){typeInt = t;}
    }
    m_SharedMainStuff->m_charStuffList[charNumber][typeInt][1] = breed;
    if(CARD_TYPE[typeInt] != "breed" && CARD_TYPE[typeInt] != "job")
    {
        m_SharedMainStuff->m_charStuffList[charNumber][typeInt][2] = object;
    }
    
    MainUser::setUserTeam(m_SharedMainStuff->m_charStuffList);
    
    initCharSpec(charNumber);
    initCardBuff(charNumber);
    return true;
}

std::vector<std::string> MainStuff::getStuffByName(int charNumber, int cardNumber)
{
    std::vector<std::string> stuffCard;
    const auto charStuffList = m_SharedMainStuff->m_charStuffList;
    for(int c = 0; c < CARD_TYPE.size(); c++)
    {
        if(charStuffList[charNumber][c][0]
           == CARD_TYPE[cardNumber])
        {
            stuffCard.push_back(charStuffList[charNumber][c][1]);
            stuffCard.push_back(charStuffList[charNumber][c][2]);
        }
    }
    return stuffCard;
}

int MainStuff::getCharUsingStuff(std::string type, std::string breed, std::string object)
{
    int characterNbr = -1;
    
    int typeInt = m_SharedMainStuff->getIndex(CARD_TYPE, type);
    
    for(int c = 0; c < CHAR_NUMBER/2; c++)
    {
        std::string breedTested = m_SharedMainStuff->m_charStuffList[c][typeInt][1];
        std::string objectTested = m_SharedMainStuff->m_charStuffList[c][typeInt][2];
        if(breedTested == breed && objectTested == object)
        {
            characterNbr = c;
            break;
        }
    }
    
    return characterNbr;
}

//---------------------------KARMA----------------------------------------------
std::string MainStuff::getKarma(int charNumber)
{
    std::string karma;
    
    const auto charStuffList = m_SharedMainStuff->m_charStuffList;
    
    int breedMax = 0;
    std::map<std::string, int> karmaList;
    std::map<std::string, int>::iterator kIt;
    for(int b = 0; b < BREED_TYPE.size(); b++)
    {
        karmaList[BREED_TYPE[b]] = 0;
    }
    for(int c = 0; c < CARD_TYPE.size(); c++)
    {
        karmaList[charStuffList[charNumber][c][1]] += 9;
        karmaList[charStuffList[charNumber][c][2]] ++;
    }
    for(kIt = karmaList.begin(); kIt != karmaList.end(); kIt++)
    {
        if(kIt->second > breedMax)
        {
            breedMax = kIt->second;
            karma = kIt->first;
        }
    }
    return karma;
}
std::string MainStuff::getTeamKarma(int teamNumber)
{
    const auto charStuffList = m_SharedMainStuff->m_charStuffList;
    int tn = teamNumber * 5;
    std::string teamKarma;
    int breedMax = 0;
    std::map<std::string, int> karmaList;
    std::map<std::string, int>::iterator kIt;
    for(int b = 0; b < BREED_TYPE.size(); b++)
    {
        karmaList[BREED_TYPE[b]] = 0;
    }
    for(int c = 0; c < CARD_TYPE.size(); c++)
    {
        for(int n = 0 + tn; n < 5 + tn; n++)
        {
            karmaList[charStuffList[n][c][1]] += 9;
            karmaList[charStuffList[n][c][2]] ++;
        }
    }
    for(kIt = karmaList.begin(); kIt != karmaList.end(); kIt++)
    {
        if(kIt->second > breedMax)
        {
            breedMax = kIt->second;
            teamKarma = kIt->first;
        }
    }
    return teamKarma;
}

//---------------------------CHARACTER SPEC--------------------------------
void MainStuff::initCharSpec(int charNbr)
{
    std::map<std::string, int> charSpec;
    std::map<std::string, int>::iterator csIt;
    charSpec["crystal_red"] = 0;
    charSpec["crystal_blue"] = 0;
    charSpec["life"] = 0;
    charSpec["level_xp"] = 0;
    charSpec["defense"] = 0;
    charSpec["attack"] = 0;
    
    for(int t = 4; t < CARD_TYPE.size(); t++)
    {
        const int nbrSlot = 3;
        for(int s = 0; s < nbrSlot; s++)
        {
            std::vector<std::string> stuff = getStuffByName(charNbr, t);
            std::string cardSlotName  = getCardSpec(CARD_TYPE[t], stuff[0], stuff[1], charNbr)->getSlot("slot" + std::to_string(s));
            
            for(csIt = charSpec.begin(); csIt != charSpec.end(); csIt++)
            {
                if(cardSlotName == csIt->first)
                {
                    charSpec[csIt->first]++;
                    if(charSpec["crystal_red"] > 6){charSpec["crystal_red"] = 6;}
                    if(charSpec["crystal_blue"] > 6){charSpec["crystal_blue"] = 6;}
                    if(charSpec["life"] > 9){charSpec["life"] = 9;}
                    if(charSpec["defense"] > 12){charSpec["defense"] = 12;}
                    if(charSpec["attack"] > 12){charSpec["attack"] = 12;}
                }
            }
        }
    }
    charSpec["crystal"] = charSpec["crystal_red"] + charSpec["crystal_blue"];
    charSpec["health"] = charSpec["life"];
    charSpec["xp"] = charSpec["level_xp"];
    charSpec["shield"] = charSpec["defense"];
    charSpec["force"] = charSpec["attack"];
    
    m_SharedMainStuff->m_charactersSpec[charNbr] = charSpec;
}
void MainStuff::setCharSpec(int charNbr, std::string specName, int value)
{
    auto charSpec = m_SharedMainStuff->m_charactersSpec[charNbr];
    
    int specValue = charSpec[specName] + value;
    
    if(charSpec[specName] + value <= 0){specValue = 0;}
    
    const int cRedBlue = charSpec["crystal_red"] + charSpec["crystal_blue"];
    if(specName == "crystal" && specValue > cRedBlue){specValue = cRedBlue;}
    if(specName == "health" && specValue > charSpec["life"]){specValue = charSpec["life"];}
    if(specName == "shield" && specValue > 12){specValue = 12;}
    if(specName == "force" && specValue > 12){specValue = 12;}
    if(specName == "level_xp" && specValue > LEVELS_NUMBER - 1){specValue = LEVELS_NUMBER -1;}
    if(specName == "xp" && charSpec["level_xp"] >= 1 && specValue >= LEVELS[charSpec["level_xp"]])
    {
            specValue = specValue - LEVELS[charSpec["level_xp"]];
            setCharSpec(charNbr, "level_xp", 1);
    }
    
    if(m_SharedMainStuff->m_charactersSpec[charNbr][specName] != specValue)
    {
        MainObject::getCharByNumber(charNbr)->setInfo(specName, value);
        m_SharedMainStuff->m_charactersSpec[charNbr][specName] = specValue;
    }
}
std::map<std::string, int> MainStuff::getCharSpec(int charNbr)
{
    if(m_SharedMainStuff->m_charactersSpec[charNbr].size() <= 0)
    {
        initCharSpec(charNbr);
    }
    return m_SharedMainStuff->m_charactersSpec[charNbr];
}
bool MainStuff::initCombatSpecs(int charNbr)
{
    m_SharedMainStuff->m_charactersSpec[charNbr]["shield"] = m_SharedMainStuff->m_charactersSpec[charNbr]["defense"];
    m_SharedMainStuff->m_charactersSpec[charNbr]["force"] = m_SharedMainStuff->m_charactersSpec[charNbr]["attack"];
    
    return true;
}

//---------------------------CHARACTER BUFF----------------------------------
bool MainStuff::initCardBuff(int charNbr)
{
    std::map<std::string, int>::iterator cbIt;
    for(cbIt = m_SharedMainStuff->m_cardBuff[charNbr].begin(); cbIt != m_SharedMainStuff->m_cardBuff[charNbr].end(); cbIt++)
    {
        //remove character's crystal buffs.
        std::size_t crystal = cbIt->first.find("crystal");
        if(crystal != std::string::npos)
        {
            cbIt->second = 0;
        }
    }
    
    for(int t = 0; t < m_SharedMainStuff->m_charStuffList[charNbr].size(); t++)
    {
        KFSpecCard* specCard = getCardSpec(CARD_TYPE[t], m_SharedMainStuff->m_charStuffList[charNbr][t][1], m_SharedMainStuff->m_charStuffList[charNbr][t][2], charNbr);
        if(specCard)
        {
            int nbrSlot = 3;
            for(int s = 0; s < nbrSlot ; s++)
            {
                //add cards crystal buffs
                std::string slotName = specCard->getSlot("slot" + std::to_string(s));
                std::size_t crystal = slotName.find("crystal");
                
                if(crystal != std::string::npos &&
                   slotName.compare(8,4,"blue") != 0 &&
                   slotName.compare(8,3,"red") != 0 &&
                   slotName.compare(8,5,"break") != 0)
                {
                    m_SharedMainStuff->setCardBuff(charNbr, slotName, -1);
                }
            }
        }
    }
    return true;
}
bool MainStuff::setCardBuff(int charNbr, std::string buffName, int value)
{
    std::map<std::string, int>::iterator cbIt;
    cbIt = m_SharedMainStuff->m_cardBuff[charNbr].find(buffName);
    if(cbIt != m_SharedMainStuff->m_cardBuff[charNbr].end())
    {
        cbIt->second += value;
        if(cbIt->second == 0)
        {
            m_SharedMainStuff->m_cardBuff[charNbr].erase(cbIt);
        }
    }
    else
    {
        m_SharedMainStuff->m_cardBuff[charNbr][buffName] = value;
    }
    
    return true;
}
std::map<std::string, int> MainStuff::getCardBuff(int charNbr)
{
    return m_SharedMainStuff->m_cardBuff[charNbr];
}

bool MainStuff::setLineBuff(int charNbr, int charLine)
{
    if(charNbr >= 5)
    {
        charLine = 4 + charLine * -1;
    }
        
    if(getKarma(charNbr) == getTeamKarma(1))
    {
        
        if(charLine == 4)
        {
            m_SharedMainStuff->setCharSpec(charNbr, "force", 2);
        }
        if(charLine == 3)
        {
            m_SharedMainStuff->setCharSpec(charNbr, "force", 1);
        }
    }
    if(getKarma(charNbr) == getTeamKarma(0))
    {
        if(charLine == 1)
        {
            m_SharedMainStuff->setCharSpec(charNbr, "shield", 1);
        }
        if(charLine == 0)
        {
            m_SharedMainStuff->setCharSpec(charNbr, "shield", 2);
        }
    }
    
    return true;
}

//---------------------------CARD SPEC----------------------------------
KFSpecCard* MainStuff::getCardSpec(std::string type, std::string breed, std::string object, int charNbr, bool isLevelUp)
{
    KFSpecCard* specCard = nullptr;
    
    int level = MainObject::getCharByNumber(charNbr)->getCardLevel(type);
    
    if(isLevelUp)
    {
        level +=1;
    }
    
    if(level > CARD_LEVELS_NBR -1)
    {
        level = CARD_LEVELS_NBR -1;
    }
    
    if(level >= 0)
    {
        Vector<KFSpecCard*>::iterator cslIt;
        for(cslIt = m_SharedMainStuff->m_cardSpecLibrary.begin(); cslIt != m_SharedMainStuff->m_cardSpecLibrary.end(); cslIt++)
        {
            KFSpecCard* sc = *cslIt;
            if(sc->getType() == type && sc->getBreed() == breed && sc->getObject() == object && sc->getLevel() == level)
            {
                specCard = sc;
            }
        }
    }
    
    m_SharedMainStuff->addCardSpecBuff(specCard, "crystal", charNbr);
    
    return specCard;
}

void MainStuff::addCardSpecBuff(KFSpecCard* specCard, std::string buffName, int charNbr)
{
    specCard->setCardBuff(KFSpecCard::mana, 0);
    
    std::map<std::string, int>::iterator cblIt;
    for(cblIt = m_SharedMainStuff->m_cardBuff[charNbr].begin(); cblIt != m_SharedMainStuff->m_cardBuff[charNbr].end(); cblIt++)
    {
        std::size_t crystal = cblIt->first.find(buffName);
        if(crystal != std::string::npos && cblIt->first.compare(8, cblIt->first.size() -8, specCard->getType()) == 0)
        {
            specCard->setCardBuff(KFSpecCard::mana, cblIt->second);
        }
    }
}

//-----------------------HELP FUNCTION---------------------------------
int MainStuff::getIndex(std::vector<std::string> v, std::string s)
{
    auto it = find(v.begin(), v.end(), s);
 
    if (it != v.end())
    {
        int index = int(it - v.begin());
        return index;
    }
    else
    {
        return -1;
    }
}

