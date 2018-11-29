//
// Created by hikaru on 23/11/18.
//

#include "HandleTurn.h"
#include <iostream>
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

    if(state.getCurrentPlayerId() > state.getPlayerNbr()){
        throw invalid_argument(" can't find the player ! aborting");
    }
    //incrémente le current player id pour passer au tour suivant
    state.setCurrentPlayerId();
    cout << "prochain joueur id : " << state.getCurrentPlayerId() << endl;
    state.setCurrentPlayer();
    state.setDay();
    cout << "le nouveau jour : " << state.getDay() << endl;
    //remet disponible tous les objets du joueur suivant
    state.resetAvailability();
    return true;
}

