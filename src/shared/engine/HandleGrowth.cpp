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

    unsigned int count_farm = 0, count_mine = 0;

    for(int i = 0; i < (int)state.getCurrentPlayer()->getPlayerBuildingList().size(); i ++){
        state::Building * building_i = state.getCurrentPlayer()->getPlayerBuildingList()[i].get();
        if(building_i->getBuildingType() == state::farm){
            count_farm ++;
        }
        if(building_i->getBuildingType()  == state::mine){
            count_mine ++;
        }
    }
    state.getCurrentPlayer()->setRessource(count_farm * 100 + 50, count_mine * 100 + 50);
    return true;
}

void HandleGrowth::serialize (Json::Value& out) const{

}

HandleGrowth* HandleGrowth::deserialize (Json::Value& out){

}

HandleGrowth::HandleGrowth() = default;
HandleGrowth::~HandleGrowth() = default;