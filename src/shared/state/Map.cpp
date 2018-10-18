//
// Created by ben on 15/10/18.
//
#include "Map.h"

using namespace state;

Map::Map (unsigned int X, unsigned int Y, std::vector<int> terrain_int)
{
    this->map_size_x = X;
    this->map_size_Y = Y;

}

bool Map::addGameObject (unsigned int player_id, Position pos, bool is_static, int type)
{

    return true;
}

bool Map::getTerrain (unsigned int X, unsigned int Y, Terrain * terrain)
{
    if (X > map_size_x || Y > map_size_Y)
        return false;
    terrain = &this->list_map[X + Y * map_size_x];
    return true;
}

bool Map::getGameObject (unsigned int X, unsigned int Y, std::vector<GameObject> * game_objects)
{
    for(int GOind = 0; GOind < list_game_object.size(); GOind ++)
    {
        if(list_game_object[GOind].getObject_position() == )
    }
    return true;
}

bool Map::removeGameObject (unsigned int game_object_id)
{

    return true;
}
