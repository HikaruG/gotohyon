//
// Created by hikaru on 07/11/18.
//
#include "HandleGrowth.h"

using namespace engine;
using namespace state;
using namespace std;

CommandTypeId HandleGrowth::getTypeId() const {
    return CommandTypeId::HANDLE_GROWTH;
}

bool HandleGrowth::execute(state::State &state) {

    unsigned int current_player = state.getCurrentPlayerId();
    unsigned int count_farm = 0, count_mine = 0;

            ;

    for(int i = 0; i < state.getCurrentPlayer(current_player)->getPlayerBuildingList().size(); i ++){
        state::Building * building_i = state.getCurrentPlayer(current_player)->getPlayerBuildingList()[i].get();
        if(building_i->getBuildingType() == state::farm){
            count_farm ++;
        }
        if(building_i->getBuildingType()  == state::mine){
            count_mine ++;
        }
    }
    state.getCurrentPlayer(current_player)->setRessource(count_farm * 100, count_mine * 100);
    return true;
}