//
// Created by hikaru on 08/11/18.
//

#include "HandleDamage.h"

using namespace engine;

CommandTypeId HandleDamage::getTypeId() const {
    return CommandTypeId::HANDLE_DAMAGE;
}

bool HandleDamage::execute(state::Unit &unit, state::Unit &ennemy_unit, state::Terrain &terrain) {
    state::Property * prop_unit;
    /* erreur : invalid use of icomplete type // j'ai pas cherché, trop fatigué et en colère pour ça, perso je l'avais pas à mon premier codage <3
    unit.getProperty(prop_unit);
    ennemy_unit.takeDamage(prop_unit.getAttack());
     */
}