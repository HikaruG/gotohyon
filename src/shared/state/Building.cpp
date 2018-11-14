//
// Created by ben on 15/10/18.
//
#include "Building.h"
#include "GameObject.h"
#include <iostream>

using namespace state;

Building::Building(){
    type = town;
}


Building::Building (unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property prop, state::BuildingType build_type)
:GameObject(gameobject_id,player_id,pos, prop)
{
    type = build_type;
}

const BuildingType Building::getBuildingType (){
    return this->type;
}