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
        this->list_map.push_back(move(shared_ptr<Terrain> (new Terrain(newPos, 1, static_cast<TerrainType >(terrain_int[i])))));
    }
    Event event = Event(EventTypeId::TERRAIN_CHANGED);
    notifyObservers(event);

}

//
bool Map::addGameObject(shared_ptr<GameObject> game_object) {
    shared_ptr<GameObject> new_object = game_object;
    list_game_object.push_back(new_object);
    Event event = Event(EventTypeId::BUILDING_CHANGED);
    notifyObservers(event);
    return true;
}


bool Map::deleteGameObject(state::GameObject* deleting_game_object) {
    size_t list_size = list_game_object.size();
    for(int i = 0; i < (int)list_game_object.size(); i++){
        if(deleting_game_object->getGame_object_id() == list_game_object[i].get()->getGame_object_id()) {
            //détruit l'objet situé à la position i
            list_game_object.erase(list_game_object.begin() + i);
            break;
        }
    }
    if(list_size != list_game_object.size() + 1) {
        cout << "error deleting map object" << endl;
        return false;
    }
    Event event = Event(EventTypeId::TERRAIN_CHANGED);
    notifyObservers(event);
    return true;
}


shared_ptr<Terrain> Map::getTerrain (unsigned int X, unsigned int Y)
{
    if (X > map_size_x || Y > map_size_Y)
    {
        throw std::invalid_argument("terrain size does not match x and y");
    }
    //instancie une nouvelle unique_ptr pour permettre un "unique_ptr rvalue"
    shared_ptr<Terrain> terrain = list_map[X + Y * map_size_x];
    return terrain;
}

//renvoie la liste de shared_ptr des objets se trouvant sur la position (X,Y)
vector<shared_ptr<GameObject>> Map::getGameObject (unsigned int X, unsigned int Y)
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
    return  sel_objects;
}

//retourne une référence vers la liste des objets se trouvant sur la carte
vector<shared_ptr<GameObject>>& Map::getListGameObject() {
    return list_game_object;
}


bool Map::moveGameObject (unsigned int game_object_id, Position new_position)
{
    for(unsigned int GOind = 0; GOind < list_game_object.size(); GOind ++)
    {
        if(list_game_object[GOind].get()->getGame_object_id() == game_object_id)
        {
            Event event = Event(EventTypeId::UNIT_CHANGED);
            notifyObservers(event);
            return list_game_object[GOind].get()->setPosition(new_position);
        }
    }
    return false;
}

bool Map::getSize(int &size_x, int &size_y) {
    size_x = (int) map_size_x;
    size_y = (int) map_size_Y;
    return true;
}

int Map::getObjectCount() {
    return static_cast<int>(map_size_x * map_size_Y + this->list_game_object.size());
}

Map::~Map() {
    cout << " map détruite " << this << endl;
}

Map::Map() {
    this->map_size_x = 0;
    this->map_size_Y = 0;
}