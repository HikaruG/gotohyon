//
// Created by hikaru on 23/11/18.
//

#include "HandleTurn.h"

using namespace engine;
using namespace std;
using namespace state;

HandleTurn::HandleTurn() = default;

HandleTurn::~HandleTurn() = default;

CommandTypeId HandleTurn::getTypeId() const {
    return CommandTypeId ::HANDLE_TURN;
}

//pour le moment, dés que tous les objets deviennent indisponibles, le tour se finit automatiquement
bool HandleTurn::execute(state::State &state) {

    //si il n'y a plus d'unités/batiments disponibles
    if(!state.getCurrentPlayer().get()->checkAvailability()){
        if(state.getCurrentPlayerId() > state.getPlayerNbr()){
            throw invalid_argument(" can't find the player ! aborting");
        }
        //incrémente le current player id pour passer au tour suivant
        state.setCurrentPlayerId();
        state.setCurrentPlayer();
        state.setDay();
        //remet disponible tous les objets du joueur suivant
        state.resetAvailability();
    }
    return true;
}

