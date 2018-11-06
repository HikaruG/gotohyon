//
// Created by ben on 15/10/18.
//
#include "Terrain.h"

using namespace state;

Terrain::Terrain (Position pos, unsigned int mvt_cost, TerrainType terrain_id)
{
    this->terrain_position = pos;
    this->movement_cost = mvt_cost;
    this->terrain_id = terrain_id;
}

TerrainType const Terrain::getTerrainType(){
    return terrain_id;
}

