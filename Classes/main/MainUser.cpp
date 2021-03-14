//
//  MainUser.cpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 06/09/2019.
//
#include "Constants.h"

#include "MainUser.hpp"
#include "MainStuff.hpp"

#include <iostream>

USING_NS_CC;
//-------------------------RESET-------------------------------
void MainUser::reset()
{
    auto userDefault = UserDefault::getInstance();
    
    userDefault->deleteValueForKey("USER_NAME");
    userDefault->deleteValueForKey("USER_TEAM");
}

//-------------------------USER NAME---------------------------
std::string MainUser::getUserName()
{
    auto userDefault = UserDefault::getInstance();
    std::string userName  = userDefault->getStringForKey("USER_NAME");
    if(userName == "")
    {
        srand((unsigned)time(NULL));
        for (int i = 0; i < 16; ++i) {
            userName += (char)(65+(rand() % (26)));
        }
        userDefault->setStringForKey("USER_NAME", userName);
        userDefault->flush();
    }
    return userName;
}

//-------------------------USER TEAM---------------------------
//SET
bool MainUser::setUserTeam(std::vector<std::vector<std::vector<std::string>>> userTeamList)
{
    auto userDefault = UserDefault::getInstance();
    std::stringstream stream;
    for(int c = 0; c < userTeamList.size() / 2; c++)
    {
        stream << 'c';
        for(int t = 0; t < userTeamList[c].size(); t++)
        {
            stream << 't';
            int b = getBreedInt(userTeamList[c][t][1]);
            stream << 'b' << b << 'o';
            int o = getBreedInt(userTeamList[c][t][2]);
            if(o >= 0)
                stream << o;
            else
                stream << '*';
        }
    }
    userDefault->setStringForKey("USER_TEAM", stream.str());
    userDefault->flush();
    
    return true;
}
//GET
std::vector<std::vector<std::vector<std::string>>> MainUser::getUserTeam()
{
    std::vector<std::vector<std::vector<std::string>>> charStuffList;
    
    auto userDefault = UserDefault::getInstance();
    std::string userTeamStr = userDefault->getStringForKey("USER_TEAM");
    
    if(userTeamStr == "")
    {
        std::stringstream stream;
        std::vector<std::vector<std::vector<int>>> cList;
        for(int c = 0; c < CHAR_NUMBER; c++)
        {
            stream << 'c';
            srand((unsigned)time(NULL) + c);
            for(int t = 0; t < CARD_TYPE.size(); t++)
            {
                stream << 't';
                int b = rand()% BREED_TYPE.size();
                stream << 'b' << b << 'o';
                if(CARD_TYPE[t] != "breed" && CARD_TYPE[t] != "job")
                {
                    int o = rand()% BREED_TYPE.size();
                    stream << o;
                }
                else
                    stream << '*';
            }
        }
        userTeamStr = stream.str();
        userDefault->setStringForKey("USER_TEAM", userTeamStr);
        userDefault->flush();
    }
    std::string charStuffStr = userTeamStr + userTeamStr;
    charStuffList = getTeamVec(charStuffStr);
    
    return charStuffList;
}
//----------------------------OPPSING TEAM--------------------------------
bool MainUser::setOpposingTeam(std::string opposingTeamStr)
{
    std::vector<std::vector<std::vector<std::string>>> opposingTeam = getTeamVec(opposingTeamStr);
    for(int c = 0; c < opposingTeam.size(); c++)
    {
        for(int t = 0; t < opposingTeam[c].size(); t++)
        {
            std::string type = opposingTeam[c][t][0];
            std::string breed = opposingTeam[c][t][1];
            std::string object = opposingTeam[c][t][2];
            
            int enemyCharNumber = c + CHAR_NUMBER / 2;
            MainStuff::setStuff(enemyCharNumber, type, breed, object);
        }
    }
    return true;
}

//------------------------------GET TEAM STREAM (for send multiplayer)---------------------
std::string MainUser::getUserTeamStr()
{
    auto userDefault = UserDefault::getInstance();
    std::string userTeamStr = userDefault->getStringForKey("USER_TEAM");
    
    return userTeamStr;
}

//------------------------------PRIVATE FUNCTION---------------------------
std::vector<std::vector<std::vector<std::string>>> MainUser::getTeamVec(std::string teamStr)
{
    std::vector<std::vector<std::vector<std::string>>> vecTeam;
    
    std::vector<std::string> charStr = splitString(teamStr, "c");
    for(int c = 0; c < charStr.size(); c++)
    {
        std::vector<std::vector<std::string>> stuffType;
        std::vector<std::string> typeStr = splitString(charStr[c],"t");
        for(int t = 0; t < typeStr.size(); t++)
        {
            std::string detailStr = typeStr[t];
            std::size_t loc = detailStr.find('b');
            std::string breedStr = detailStr.substr(loc+1,1);
            loc = detailStr.find('o');
            std::string objectStr = detailStr.substr(loc+1,1);
            
            std::string type = CARD_TYPE[t];
            std::string breed = BREED_TYPE[(int)std::atof (breedStr.c_str())];
            std::string object = "";
            if(objectStr.find_first_not_of("0123456789") == std::string::npos)
                object = BREED_TYPE[(int)std::atof(objectStr.c_str())];
            
            std::vector<std::string> stuffDetails;
            stuffDetails.push_back(type);
            stuffDetails.push_back(breed);
            stuffDetails.push_back(object);
            stuffType.push_back(stuffDetails);
        }
        vecTeam.push_back(stuffType);
    }
    return vecTeam;
}
std::vector<std::string> MainUser::splitString(std::string mString, std::string mChar)
{
    std::vector<std::string>result;
    std::string str = mString.substr(1, mString.size());
    while(str.size()){
        int index = str.find(mChar);
        if(index != std::string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+mChar.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}
int MainUser::getBreedInt(std::string breed)
{
    int result = -1;
    for(int i = 0; i < BREED_TYPE.size(); i++)
    {
        const std::string b = BREED_TYPE[i];
        if(b == breed)
            result = i;
    }
    return result;
}
