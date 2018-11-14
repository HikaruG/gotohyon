//
// Created by hikaru on 08/11/18.
//

#include "HandleDamage.h"

using namespace engine;

CommandTypeId HandleDamage::getTypeId() const {
    return CommandTypeId::HANDLE_DAMAGE;
}

bool HandleDamage::execute(state::GameObject & object, state::GameObject & ennemy_object, state::Terrain &terrain) {
    ennemy_object.getProperty()->takeDamage(object.getProperty()->getAttack());
    return true;
}

bool HandleDamage::execute(state::State &state) {
    return true;
}

HandleDamage::HandleDamage() = default;

HandleDamage::~HandleDamage() = default;