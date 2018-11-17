//
// Created by ben on 15/10/18.
//
#include "state.h"
#include <iostream>

using namespace state;
using namespace std;

Map::Map (unsigned int X, unsigned int Y, std::vector<int>& terrain_int)
{
    this->map_size_x = X;
    this->map_size_Y = Y;
    if (X*Y != terrain_int.size())
    {
        throw std::invalid_argument("terrain size does not match x and y");
    }
    for (unsigned int i = 0; i < terrain_int.size(); i++)
    {
        Position newPos = Position(i%Y,i/Y);
        this->list_map.push_back(move( unique_ptr<Terrain> (new Terrain(newPos, 1, static_cast<TerrainType >(terrain_int[i])))));
    }
    MapEvent event = MapEvent(MapEventId::TERRAIN_CHANGED);
    notifyObservers(event);

}

//
bool Map::addGameObject(std::vector<std::shared_ptr<state::GameObject>> list_map_object,
                        shared_ptr<GameObject> game_object) {
    list_map_object.push_back(move(game_object));
}


//il faut changer cette classe addGameObject => au moins créer les 2 listes de propriétés dans Property
/*
GameObject * Map::addGameObject (unsigned int player_id, Position pos, bool is_static, int type)
{
    Property farmer = Property("unit_farmer",10,10,100,false,3);
    Property infantry = Property("unit_infantry",10,10,100,false);
    Property archer = Property("unit_archer",10,10,100,false);

    Property mine = Property("building_mine",10,10,100,true);
    Property farm = Property("building_farm",10,10,100,true);
    Property turret = Property("building_turret",10,10,100,true);
    Property town = Property("building_town",10,10,100,true);
    Property barrack = Property("building_barrack",10,10,100,true);

    std::vector<Property> statics = {mine,farm,turret,town, barrack};
    std::vector<Property> units = {farmer,infantry, archer};

    if(is_static)
    {
        std::unique_ptr<Building> new_b (new Building(static_cast<unsigned int>(list_game_object.size()),player_id, pos, statics[type],
                                  static_cast<BuildingType>(type)));
        list_game_object.push_back(std::move(new_b));

        MapEvent event = MapEvent(MapEventId::BUILDING_CHANGED);
        notifyObservers(event);
        return new_b.get();

    }
    else {
        std::unique_ptr<Unit> new_u (new Unit(1, static_cast<unsigned int>(list_game_object.size()), player_id, pos, units[type],
                          static_cast<UnitType>(type)));
        list_game_object.push_back(std::move(new_u));
        notifyObservers(MapEvent(MapEventId::UNIT_CHANGED));
        return new_u.get();
    }
}
*/


unique_ptr<Terrain>& Map::getTerrain (unsigned int X, unsigned int Y)
{
    if (X > map_size_x || Y > map_size_Y)
    {
        throw std::invalid_argument("terrain size does not match x and y");
    }
    //instancie une nouvelle unique_ptr pour permettre un "unique_ptr rvalue"
    unique_ptr<Terrain> terrain = move(list_map[X + Y * map_size_x]);
    return terrain;


}

//renvoie la liste de shared_ptr des objets se trouvant sur la position (X,Y)
vector<shared_ptr<GameObject>>& Map::getGameObject (unsigned int X, unsigned int Y)
{
    vector<shared_ptr<GameObject>> sel_objects;
    Position looking_for = Position(X,Y);
    for(unsigned int indice_game_object = 0; indice_game_object < list_game_object.size(); indice_game_object ++)
    {
        if(list_game_object[indice_game_object].get()->getPosition() == looking_for)
        {
            sel_objects.push_back(list_game_object[indice_game_object]);
        }
    }
    return sel_objects;
}

//retourne une référence vers la liste des objets se trouvant sur la carte
vector<shared_ptr<GameObject>>& Map::getListGameObject() {
    return list_game_object;
}


bool Map::moveGameObject (unsigned int game_object_id, Position new_position)
{
    for(unsigned int GOind = 0; GOind < list_game_object.size(); GOind ++)
    {
        if(list_game_object[GOind]->getGame_object_id() == game_object_id)
        {
            MapEvent event = MapEvent(MapEventId::UNIT_CHANGED);
            notifyObservers(event);
            return list_game_object[GOind]->setPosition(new_position);
        }
    }
    return false;
}

bool Map::getSize(int *size_x, int *size_y) {
    *size_x = (int) map_size_x;
    *size_y = (int) map_size_Y;
    return true;
}

int Map::getObjectCount() {
    return static_cast<int>(map_size_x * map_size_Y + this->list_game_object.size());
}

Map::~Map() {

}

Map::Map() {
    this->map_size_x = 0;
    this->map_size_Y = 0;
}