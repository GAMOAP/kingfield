//
//  Constants.h
//  kingfield
//
//  Created by alexandre gimeno on 23/09/2019.
//

#ifndef Constants_h
#define Constants_h

//------------------------------GLOBAL CONSTANT---------------------------
const bool RESET_MEMORY = false;

const bool TEST_SCENE = false;

const bool TEST_CHAR_ANIM_ON = false;

const bool TEST_CARD = false;

const bool TEST_NET_ON = false;
const bool MULTI_PLAYER_ON = false;

const int CHAR_NUMBER = 10;

const int LEVELS_NUMBER = 6;
const int LEVELS[LEVELS_NUMBER] = {0, 3, 7, 12, 18, 25};
const int CARD_LEVELS_NBR = 3;

const std::vector<std::string> CARD_TYPE = {"move","spell","weapon","object","item","armor","helmet","job","breed"};

const std::vector<std::string> BREED_TYPE = {"sun","night","time"};

const std::vector<std::string> CHAR_SLOTS = {"head","face","hand","helmet","armor","arm","frontLeg","backLeg","weapon","item_arm","item_armor","item_hand"};

const std::vector<std::string> CHAR_EXPRESSION = {"pain", "sleep", "sick", "tired", "happy", "blink", "angry"};

const std::vector<std::string> SPECS = {"crystal_red","crystal_blue","life","defense","attack","crystal_move","crystal_spell","crystal_weapon","crystal_object","crystal_break","defense_more","attack_more","defense_less","attack_less","poison","sleep","block","heal"};

#endif /* Constants_h */
