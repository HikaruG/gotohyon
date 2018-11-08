//
// Created by hikaru on 07/11/18.
//

#include "HandleCreation.h"

using namespace engine;

CommandTypeId HandleCreation::getTypeId() const {
    return CommandTypeId::HANDLE_CREATION;
}

bool HandleCreation::execute(state::State &state, state::Unit &unit, state::Building &building, unsigned int pos_x, unsigned int pos_y) {
    return true;
}















