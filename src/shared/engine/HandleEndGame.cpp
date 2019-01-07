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
    if(state.isGameFinished())
        return true;
    return false;
}
