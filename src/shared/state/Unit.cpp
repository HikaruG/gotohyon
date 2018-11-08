//
// Created by ben on 15/10/18.
//
#include "Unit.h"

using namespace state;

Unit::Unit (unsigned int movement_range, unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property property, UnitType unit_type)
:GameObject(gameobject_id,player_id,pos, property)
{
    this->movement_range = movement_range;
    this->type = unit_type;
    this->is_static = false;
}
UnitType const Unit::getUnitType ()
{
    return type;
}

int Unit::getMovementRange() {
    return movement_range;
}