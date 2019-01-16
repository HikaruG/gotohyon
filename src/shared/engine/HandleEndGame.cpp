//
// Created by hikaru on 09/12/18.
//

#include "HandleEndGame.h"
#include <iostream>

using namespace engine;

HandleEndGame::HandleEndGame() = default;

HandleEndGame::~HandleEndGame() = default;

CommandTypeId HandleEndGame::getTypeId() const {
    return CommandTypeId::HANDLE_ENDGAME;
}

bool HandleEndGame::execute(state::State &state) {
    return state.isGameFinished();
}


bool HandleEndGame::undo(state::State &state) {
    return true;
}


void HandleEndGame::serialize (Json::Value& out) const{
    out["CommandId"]=this->getTypeId();
}

HandleEndGame* HandleEndGame::deserialize (Json::Value& out){
    return this;
}