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
    //counting nbr of objects:
    state::Map * current_map = this->current_state->getMap();

    current_map->getObjectCount();
    int tmp_x;
    int tmp_y;
    current_map->getSize(&tmp_x,&tmp_y);

    this->map_size_x = (unsigned int&) tmp_x;
    this->map_size_y = (unsigned int&) tmp_y;
    drawer = MapSurface();
    if(!drawer.loadTextures("res/tileset_terrain.png","res/tileset_unit.png","res/tileset_building.png"))
        std::cout<<"Error: Textures failed to load, launch like this: bin/client whatever" << std::endl;
    updateState(current_state);




}


bool DrawManager::updateState(state::State &new_state) {
    this->current_state = &new_state;
    window->clear(sf::Color::Black);


    setTerrain();
    setBuilding();
    setUnit();

    window->display();

    return true;
}


bool DrawManager::setTerrain ()
{
    state::Map * current_map = this->current_state->getMap();
    state::Terrain * local_terrain = new state::Terrain();
    drawer.initQuads(this->map_size_x+this->map_size_x*this->map_size_y);
    for(unsigned int x = 0; x < this->map_size_x;x ++)
    {
        for(unsigned int y=0; y< this->map_size_y;y++)
        {
            current_map->getTerrain(x,y,local_terrain);
            drawer.setSpriteLocation(x+y*map_size_x,x,y);
            //std::cout<<"adding "<<local_terrain->getTerrainType()<<" as "<<local_terrain->getTerrainType()<<std::endl;
            drawer.setSpriteTexture(0,local_terrain->getTerrainType(),x+y*map_size_x);
        }
    }
    drawer.draw(*window,state_sfml);
    return true;
}

bool DrawManager::setUnit ()
{
    state::Map * current_map = new state::Map();
    current_map = this->current_state->getMap();
    current_map->getListGameObject(game_object_list);
    std::vector<state::Unit *> units_go;
    state::Position pos;
    for (unsigned int i = 0; i< game_object_list.size();i++)
    {
        if(!game_object_list[i]->isStatic())
        {
            units_go.push_back((state::Unit*)game_object_list[i]);
        }

    }
    drawer.initQuads(static_cast<int>(units_go.size()));
    for(unsigned int i = 0; i<units_go.size(); i++)
    {
        drawer.setSpriteTexture(2,units_go[i]->getUnitType(), i);
        pos = units_go[i]->getPosition();
        drawer.setSpriteLocation(i,pos.getX(), pos.getY());
    }

    drawer.draw(*window,state_sfml);
    return true;
}

bool DrawManager::setBuilding ()
{

    state::Map * current_map = this->current_state->getMap();
    current_map->getListGameObject(game_object_list);
    std::vector<state::Building *> build_go;
    state::Position pos;
    for (unsigned int i = 0; i< game_object_list.size();i++)
    {
        if(game_object_list[i]->isStatic())
        {
            build_go.push_back((state::Building*)game_object_list[i]);
        }

    }
    drawer.initQuads(build_go.size());
    for(unsigned int i = 0; i<build_go.size(); i++)
    {
        drawer.setSpriteTexture(1,build_go[i]->getBuildingType(), i);
        pos = build_go[i]->getPosition();
        drawer.setSpriteLocation(i,pos.getX(), pos.getY());
    }

    drawer.draw(*window,state_sfml);
    return true;
}