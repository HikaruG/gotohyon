//
// Created by ben on 15/10/18.
//
#include "Building.h"
#include <iostream>

using namespace state;

Building::Building (unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property prop, state::BuildingType build_type)
:GameObject(gameobject_id,player_id,pos, prop)
{
    if(prop.getAttack() == 0)
    {
        can_attack = false;
    }
    else{
        can_attack = true;
    }
    type = build_type;
    this->is_static = true;
}

const BuildingType Building::getBuildingType (){
    return this->type;
}