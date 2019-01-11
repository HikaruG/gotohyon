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

//pour le moment, le tour se finit automatiquement dés qu'on lance cette commande
bool HandleTurn::execute(state::State &state) {
    cout << "the current player id is " << state.getCurrentPlayerId() << endl;
    if(state.getCurrentPlayerId() > state.getPlayerNbr()){
        throw invalid_argument(" can't find the player ! aborting");
        return false;
    }

    //vérifie les conditions de fin de game
    //test seulement si le jour 0 est passé (jour d'initialisation)
    if((int)state.getDay() > 0) {
        for (int i = 0; i < (int)state.getListPlayer().size(); i++) {
            if (state.getListPlayer()[i].get()->getIsDead()) {
                state.killPlayer(state.getListPlayer()[i].get()->getPlayerId());
            }
        }
        if(state.isGameFinished()){
            return true;
        }
        if ((int)state.getListPlayer().size() != state.getRemainingPlayers()) {
            throw invalid_argument("error deleting a player ");
        }
        else {
            if((int)state.getRemainingPlayers() < 2){
                cout << "the game is over !" << endl;
                return false;
            }
        }
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


bool HandleTurn::undo(state::State &state) {
    state.setCurrentPlayerId(false);
    state.setCurrentPlayer();
    state.setDay(false);
    return true;
}


void HandleTurn::serialize (Json::Value& out) const{
    out["CommandId"]=9;
    out["CurrentPlayerId"]=this->current_player_id;
    out["CurrentDay"]=this->current_day;
}

HandleTurn* HandleTurn::deserialize (Json::Value& out){
    this->current_player_id=out.get("CurrentPlayerId","error").asUInt();
    this->current_day=out.get("CurrentDay","error").asUInt();
    return this;
}