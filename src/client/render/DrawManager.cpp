//
// Created by ben on 25/10/18.
//
#include "DrawManager.h"
#include "state.h"

using namespace render;

DrawManager::DrawManager (state::State& current_state):
current_state(current_state)
{

}

bool DrawManager::getSurface (Surface * surface)
{

    return true;
}
bool DrawManager::getTerrains ()
{

    return true;
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