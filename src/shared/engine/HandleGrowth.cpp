//
// Created by hikaru on 07/11/18.
//
#include "HandleGrowth.h"

using namespace engine;

CommandTypeId HandleGrowth::getTypeId() const {
    return CommandTypeId::HANDLE_GROWTH;
}

bool HandleGrowth::execute(state::State &state) {

    unsigned int count_farm = 0, count_mine = 0;
    std::vector<state::Building *> building_list;
    state.getCurrentPlayer()->getPlayerBuildingList(building_list);

// erreur : invalid use of incomplete type // j'ai pas cherché, trop fatigué et en colère pour ça, perso je l'avais pas à mon premier codage <3
    for(state::Building * s : building_list){
        if(s->getBuildingType() == state::farm){
            count_farm ++;
        }
        if(s->getBuildingType() == state::mine){
            count_mine ++;
        }
    }

    state.getCurrentPlayer()->setRessource(count_farm * 100, count_mine * 100);
}