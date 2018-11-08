//
// Created by hikaru on 07/11/18.
//

#include "HandleCreation.h"

using namespace engine;

CommandTypeId HandleCreation::getTypeId() const {
    return CommandTypeId::HANDLE_CREATION;
}

bool HandleCreation::execute(state::State &state, unsigned int pos_x, unsigned int pos_y, int type, bool is_static) {
    state::Position position(pos_x, pos_y);
    state.addGameObject(state.getCurrentPlayer()->getPlayerId(),position, is_static, type);
    return true;
}















