//
// Created by ben on 15/10/18.
//
#include <iostream>
#include "Map.h"
#include "Building.h"
#include "Unit.h"
#include <iostream>
using namespace state;

Map::Map (unsigned int X, unsigned int Y, std::vector<int>& terrain_int)
{
    this->map_size_x = X;
    this->map_size_Y = Y;
    if (X*Y != terrain_int.size())
    {
        throw std::invalid_argument("terrain size does not match x and y");
    }
    for (int i = 0; i < terrain_int.size(); i++)
    {
        Position newPos = Position(i%Y,i/Y);
        list_map.push_back(Terrain(newPos, 1, static_cast<TerrainType >(terrain_int[i])));
    }

}

GameObject * Map::addGameObject (unsigned int player_id, Position pos, bool is_static, int type)
{
    Property farmer = Property("unit_farmer",10,10,100,3);
    Property infantry = Property("unit_infantry",10,10,100);
    Property archer = Property("unit_archer",10,10,100);

    Property mine = Property("building_mine",10,10,100);
    Property farm = Property("building_farm",10,10,100);
    Property turret = Property("building_turret",10,10,100);
    Property town = Property("building_town",10,10,100);
    Property barrack = Property("building_barrack",10,10,100);

    std::vector<Property> statics = {mine,farm,turret,town, barrack};
    std::vector<Property> units = {farmer,infantry, archer};
    
    if(is_static)
    {
        Building * new_b = new Building(static_cast<unsigned int>(list_game_object.size()),player_id, pos, statics[type],
                                  static_cast<BuildingType>(type));

        list_game_object.push_back(new_b);
        return new_b;
    }
    else {
        Unit * new_u = new Unit(10, static_cast<unsigned int>(list_game_object.size()), player_id, pos, units[type],
                          static_cast<UnitType>(type));

        list_game_object.push_back(new_u);
        return new_u;
    }
}

bool Map::getTerrain (unsigned int X, unsigned int Y, Terrain * terrain_bob)
{
    if (X > map_size_x || Y > map_size_Y)
        return false;
    *terrain_bob = this->list_map[X + Y * map_size_x];
    return true;
}

bool Map::getGameObject (unsigned int X, unsigned int Y, std::vector<GameObject*> * game_objects)
{
    Position looking_for = Position(X,Y);
    for(unsigned int GOind = 0; GOind < list_game_object.size(); GOind ++)
    {
        if(list_game_object[GOind]->getPosition() == looking_for)
        {
            game_objects->push_back(list_game_object[GOind]);
        }
    }
    return true;
}

bool Map::getListGameObject(std::vector<state::GameObject *> &list_objects) {
    list_objects = this->list_game_object;
    return true;
}

bool Map::moveGameObject (unsigned int game_object_id, Position new_position)
{
    for(int GOind = 0; GOind < list_game_object.size(); GOind ++)
    {
        if(list_game_object[GOind]->getGame_object_id() == game_object_id)
        {
            return list_game_object[GOind]->setPosition(new_position);
        }
    }
    return false;
}

bool Map::getSize(int *size_x, int *size_y) {
    std::cout << map_size_x << map_size_Y<<std::endl;
    *size_x = (int) map_size_x;
    *size_y = (int) map_size_Y;
    return true;
}

int Map::getObjectCount() {
    return (int) map_size_x * map_size_Y + this->list_game_object.size();
}

Map::~Map() {
    for (int i = 0; i >list_game_object.size();i++)
    {
        delete list_game_object[i];
    }
}

Map::Map() {
    this->map_size_x = 0;
    this->map_size_Y = 0;
}