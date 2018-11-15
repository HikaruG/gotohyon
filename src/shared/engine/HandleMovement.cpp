//
// Created by hikaru on 06/11/18.
//

#include "HandleMovement.h"
#include <iostream>

using namespace engine;

//HandleMovement::~HandleMovement() {}

CommandTypeId HandleMovement::getTypeId() const {
    return CommandTypeId::HANDLE_MOVEMENT;
}

bool HandleMovement::execute(state::Unit &unit, state::State& state, unsigned int new_x, unsigned int new_y) {
    if( (int)(unit.getPosition().getX()  + unit.getPosition().getY() - (new_x + new_y) ) < unit.getMovementRange() ){
        if(new_x < 15 && new_y < 15) {
            state::Position position(new_x, new_y);
            if (state.getMap()->moveGameObject(unit.getGame_object_id(), position)) {
                std::cout << "successfully moved" << std::endl;
            }
        }
    }

    return true;
}

bool HandleMovement::execute (state::State& state)
{
    return false;
}

HandleMovement::~HandleMovement() {

}

HandleMovement::HandleMovement() {

}
