//
// Created by ben on 25/10/18.
//
#include "DrawManager.h"
#include "state.h"

using namespace render;

DrawManager::DrawManager (state::State& current_state)
{
    this->current_state = &current_state;
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