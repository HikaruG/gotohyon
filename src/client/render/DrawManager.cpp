//
// Created by ben on 25/10/18.
//
#include "DrawManager.h"
#include "state.h"
#include <iostream>
using namespace render;

DrawManager::DrawManager (state::State& current_state, sf::RenderWindow& window)
{
    this->current_state = &current_state;
    this->window = &window;
    window.clear(sf::Color::Green);
    //counting nbr of objects:
    /*state::Map * current_map = new state::Map(1,1,{0});
    this->current_state->getMap(current_map);

    current_map->getObjectCount();
    int tmp_x;
    int tmp_y;
    std::cout <<"?"<<std::endl;
    current_map->getSize(&tmp_x,&tmp_y);
    std::cout <<"?"<<std::endl;

    this->map_size_x = (unsigned int&) tmp_x;
    this->map_size_y = (unsigned int&) tmp_y;
    std::cout << "debug size : "<<map_size_x<<" "<<map_size_y<<std::endl;*/
    drawer = MapSurface();
    if(!drawer.loadTextures("res/tileset_terrain.png","res/tileset_building.png","res/tileset_unit.png"))
        std::cout<<"Error: Textures failed to load, launch like this: bin/client whatever" << std::endl;
    updateState(current_state);



}


bool DrawManager::updateState(state::State &new_state) {
    this->current_state = &new_state;


    setTerrain();
    setBuilding();
    setUnit();



    return true;
}


bool DrawManager::setTerrain ()
{
    state::Map * current_map = new state::Map(1,1,{0});
    this->current_state->getMap(current_map);
    state::Terrain * local_terrain = new state::Terrain();
    for(unsigned int x = 0; x < this->map_size_x;x ++)
    {
        for(unsigned int y=0; y< this->map_size_y;y++)
        {
            current_map->getTerrain(x,y,local_terrain);
            drawer.setSpriteLocation(x+y*map_size_y,x,y);
            drawer.setSpriteTexture(0,local_terrain->getTerrainType());
        }
    }
    drawer.draw(*window,state_sfml);
    return true;
}

bool DrawManager::setUnit ()
{
    state::Map * current_map = new state::Map(1,1,{0});
    this->current_state->getMap(current_map);
    current_map->getListGameObject(game_object_list);
    std::vector<state::Unit *> units_go;
    int lastUnit = 0;
    state::Position pos;
    for (int i = 0; i< game_object_list.size();i++)
    {
        if(!game_object_list[i]->isStatic())
        {
            units_go.push_back((state::Unit*)game_object_list[i]);
            drawer.setSpriteTexture(2,units_go[lastUnit]->getUnitType());
            pos = units_go[lastUnit]->getPosition();
            drawer.setSpriteLocation(lastUnit,pos.getX(), pos.getY());
            lastUnit++;
        }

    }
    drawer.draw(*window,state_sfml);
    return true;
}

bool DrawManager::setBuilding ()
{

    state::Map * current_map = new state::Map(1,1,{0});
    this->current_state->getMap(current_map);
    current_map->getListGameObject(game_object_list);
    std::vector<state::Building *> build_go;
    int lastUnit = 0;
    state::Position pos;
    for (int i = 0; i< game_object_list.size();i++)
    {
        if(!game_object_list[i]->isStatic())
        {
            build_go.push_back((state::Building*)game_object_list[i]);
            drawer.setSpriteTexture(2,build_go[lastUnit]->getBuildingType());
            pos = build_go[lastUnit]->getPosition();
            drawer.setSpriteLocation(lastUnit,pos.getX(), pos.getY());
            lastUnit++;
        }

    }
    drawer.draw(*window,state_sfml);
    return true;
}