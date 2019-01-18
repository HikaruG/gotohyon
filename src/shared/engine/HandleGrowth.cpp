//
// Created by hikaru on 07/11/18.
//
#include "HandleGrowth.h"

using namespace engine;
using namespace state;
using namespace std;


HandleGrowth::HandleGrowth(unsigned int current_food, unsigned int current_gold){
    this->current_food = current_food;
    this->current_gold = current_gold;
}

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


bool HandleGrowth::undo(state::State &state) {
    state.getCurrentPlayer().get()->setRessourceNet(current_food, current_gold);
    return true;
}


void HandleGrowth::serialize (Json::Value& out) const{
    out["CommandId"]=this->getTypeId();
    out["Food"]=this->current_food;
    out["Gold"]=this->current_gold;
}

HandleGrowth* HandleGrowth::deserialize (Json::Value& out){
    this->current_food=out.get("Food",0).asUInt();
    this->current_gold=out.get("Gold",0).asUInt();
    return this;
}

HandleGrowth::~HandleGrowth() = default;

HandleGrowth::HandleGrowth() = default;