//
// Created by ben on 25/10/18.
//
#include "DrawManager.h"
#include "state.h"
#include <iostream>
using namespace render;

DrawManager::DrawManager (state::State& current_state)
{
    this->current_state = &current_state;
    //counting nbr of objects:
    state::Map * current_map = new state::Map(1,1,{0});
    current_state.getMap(current_map);
    current_map->getObjectCount();
    int * tmp_x;
    int * tmp_y;
    current_map->getSize(tmp_x,tmp_y);
    this->map_size_x = *tmp_x;
    this->map_size_y = *tmp_y;
    std::cout << "debug size : "<<map_size_x<<" "<<map_size_y<<std::endl;
    drawer = MapSurface();
    //beaucoup trop de hardcodage mais bon, a modifié plus tard !
    
}


bool DrawManager::setTerrain ()
{

    return true;
}

bool DrawManager::setUnit ()
{

    return true;
}

bool DrawManager::setBuilding ()
{

    return true;
}