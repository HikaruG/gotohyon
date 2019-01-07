#include "DeepAI.h"
#include <vector>

using namespace state;
using namespace std;
using namespace ai;

DeepAI::DeepAI() = default;

DeepAI::~DeepAI() = default;


int DeepAI::path_finder(state::State &state, state::Unit &unit) {
    int movement_range = unit.getMovementRange();
    std::vector<Unit*> list_units = {};
    for( shared_ptr<Player> players : state.getListPlayer()){
        if(players.get()->getPlayerId() != unit.getPlayerId()) {
            for(shared_ptr<Unit> units : players.get()->getPlayerUnitList()){
                list_units.push_back(units.get());
            }
        }
    }

    // archer > infantry > barrack
    // s'éloigner le plus possible de ces unités, tout en gardant à l'esprit un spot stratégique pour les batiments:
    // les batiments d'économies = le plus loin du centre ville/unités ennemie
    // les batiments de combats: le plus proche/le plus protecteur par rapport au centre ville


    switch(unit.getUnitType()){
        case farmer:
            for(int i = movement_range; i >= 0; i--){

            }
            break;


        case archer:
            break;


        case infantry:
            break;

        default:
            return false;
    }
    return 0;
}

bool DeepAI::run(engine::Engine &engine, state::State &state) {
    return true;
}
