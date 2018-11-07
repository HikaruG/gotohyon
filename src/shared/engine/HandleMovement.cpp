//
// Created by hikaru on 06/11/18.
//

#include "HandleMovement.h"

using namespace engine;

//HandleMovement::~HandleMovement() {}

CommandTypeId HandleMovement::getTypeId() const {
    return CommandTypeId::HANDLE_MOVEMENT;
}

bool HandleMovement::execute(state::Terrain &terrain, state::Unit &unit, std::vector<state::Terrain *> &list_terrain, state::State& state) {
    unsigned int unit_x, unit_y;
    unit.getPosition().getPosition(&unit_x, &unit_y);
    return true;
}