//
// Created by hikaru on 06/11/18.
//

#include "HandleMovement.h"

using namespace engine;

//HandleMovement::~HandleMovement() {}

CommandTypeId HandleMovement::getTypeId() const {
    return CommandTypeId::HANDLE_MOVEMENT;
}

bool HandleMovement::execute(state::Unit &unit, state::State& state, unsigned int new_x, unsigned int new_y) {
    if( (unit.getPosition().getX()  + unit.getPosition().getY() - (new_x + new_y) ) < unit.getMovementRange() ){
        state::Position position(new_x,new_y);
        unit.setPosition(position);
    }

    return true;
}