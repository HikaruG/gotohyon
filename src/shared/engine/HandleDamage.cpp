//
// Created by hikaru on 08/11/18.
//

#include "HandleDamage.h"

using namespace engine;

CommandTypeId HandleDamage::getTypeId() const {
    return CommandTypeId::HANDLE_DAMAGE;
}

bool HandleDamage::execute(state::Unit & unit, state::Unit & ennemy_unit, state::Terrain &terrain) {
    ennemy_unit.getProperty()->takeDamage(unit.getProperty()->getAttack());
    return true;
}

bool HandleDamage::execute(state::State &state) {
    return true;
}

HandleDamage::HandleDamage() = default;

HandleDamage::~HandleDamage() = default;