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

Terrain::Terrain() {
    Position default_pos = Position(0,0);
    this->terrain_position = default_pos;
    this->movement_cost =1;
    this->terrain_id = 0;
}

TerrainType const Terrain::getTerrainType(){
    return terrain_id;
}

