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
    
    std::string filePath = "res/json/card_spec.json";
    std::string prevPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    const auto data = cocos2d::FileUtils::getInstance()->getStringFromFile(prevPath);
        
    for(int t = 0; t < CARD_TYPE.size(); t++)
    {
        for(int b = 0; b < BREED_TYPE.size(); b++)
        {
            KFSpecCard* stuffCard = nullptr;
            if(CARD_TYPE[t] == "breed" || CARD_TYPE[t] == "job")
            {
                stuffCard = KFSpecCard::createCard(CARD_TYPE[t], BREED_TYPE[b]);
                m_cardSpecLibrary.push_back(stuffCard);
            }
            else
            {
                for(int o = 0; o < BREED_TYPE.size(); o++)
                {
                    stuffCard = KFSpecCard::createCard(CARD_TYPE[t], BREED_TYPE[b], BREED_TYPE[o]);
                    m_cardSpecLibrary.push_back(stuffCard);
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
    initCharBuff(charNumber);
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
                    if(charSpec["life"] > 12){charSpec["life"] = 12;}
                    if(charSpec["defense"] > 12){charSpec["defense"] = 12;}
                    if(charSpec["attack"] > 12){charSpec["attack"] = 12;}
                }
            }
        }
    }
    charSpec["crystal"] = charSpec["crystal_red"] + charSpec["crystal_blue"];
    charSpec["health"] = charSpec["life"];
    charSpec["shield"] = charSpec["defense"];
    charSpec["strike"] = charSpec["attack"];
    
    m_SharedMainStuff->m_charactersSpec[charNbr] = charSpec;
}
void MainStuff::setCharSpec(int charNbr, std::string specName, int value)
{
    auto charSpec = m_SharedMainStuff->m_charactersSpec[charNbr];
    
    int specValue = charSpec[specName] + value;
    
    if(charSpec[specName] + value < 0){specValue = 0;}
    
    const int cRedBlue = charSpec["crystal_red"] + charSpec["crystal_blue"];
    if(specName == "crystal" && specValue > cRedBlue){specValue = cRedBlue;}
    if(specName == "health" && specValue > charSpec["life"]){specValue = charSpec["life"];}
    if(specName == "shield" && specValue > 12){specValue = 12;}
    if(specName == "strike" && specValue > 12){specValue = 12;}
    
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

//---------------------------CHARACTER BUFF----------------------------------
bool MainStuff::initCharBuff(int charNbr)
{
    std::vector<StuffBuff>::iterator scbIt;
    for(scbIt = m_SharedMainStuff->m_charactersBuff[charNbr].begin(); scbIt != m_SharedMainStuff->m_charactersBuff[charNbr].end(); scbIt++)
    {
        //remove character's crystal buffs.
        std::size_t crystal = scbIt->name.find("crystal");
        if(crystal != std::string::npos)
        {
            scbIt->value = NULL;
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
                    m_SharedMainStuff->setCharBuff(charNbr, slotName, 1);
                }
            }
        }
    }
    return true;
}
bool MainStuff::setCharBuff(int charNbr, std::string buffName, int value, int turn)
{
    bool buffFinded = false;
    for(int i = 0; i < m_SharedMainStuff->m_charactersBuff[charNbr].size(); i++)
    {
        StuffBuff stuffBuff = m_SharedMainStuff->m_charactersBuff[charNbr][i];
        
        if(stuffBuff.name == buffName)
        {
            stuffBuff.value += value;
            buffFinded = true;
        }
        if(stuffBuff.value <= 0)
        {
            m_SharedMainStuff->m_charactersBuff[charNbr].erase(m_SharedMainStuff->m_charactersBuff[charNbr].begin() + i);
        }
    }
    
    if(!buffFinded)
    {
        StuffBuff newStuffBuff;
        newStuffBuff.name = buffName;
        newStuffBuff.value = value;
        newStuffBuff.turn = turn;
    
        m_SharedMainStuff->m_charactersBuff[charNbr].push_back(newStuffBuff);
    }
    return true;
}
std::vector<StuffBuff> MainStuff::getCharBuff(int charNbr)
{
    return m_SharedMainStuff->m_charactersBuff[charNbr];
}

//---------------------------CARD SPEC----------------------------------
KFSpecCard* MainStuff::getCardSpec(std::string type, std::string breed, std::string object, int charNbr)
{
    KFSpecCard* specCard = nullptr;
    
    Vector<KFSpecCard*>::iterator cslIt;
    for(cslIt = m_SharedMainStuff->m_cardSpecLibrary.begin(); cslIt != m_SharedMainStuff->m_cardSpecLibrary.end(); cslIt++)
    {
        KFSpecCard* sc = *cslIt;
        if(sc->getType() == type && sc->getBreed() == breed && sc->getObject() == object)
        {
            specCard = sc;
        }
    }
    
    std::vector<StuffBuff>::iterator cblIt;
    for(cblIt = m_SharedMainStuff->m_charactersBuff[charNbr].begin(); cblIt != m_SharedMainStuff->m_charactersBuff[charNbr].end(); cblIt++)
    {
        std::size_t crystal = cblIt->name.find("crystal");
        if(crystal != std::string::npos && cblIt->name.compare(8, cblIt->name.size() -8, type) == 0)
        {
            specCard->setCardBuff(KFSpecCard::mana, cblIt->value);
        }
    }
    
    return specCard;
}
