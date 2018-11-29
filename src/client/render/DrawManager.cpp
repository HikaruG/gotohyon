//
// Created by ben on 25/10/18.
//
#include "DrawManager.h"
#include "state.h"
#include "render.h"
#include <iostream>
using namespace render;
using namespace std;

DrawManager::DrawManager ( shared_ptr<state::State> current_state, sf::RenderWindow& window)
:terrain_layer(OnMapLayer("res/tileset_terrain.png")),building_layer(OnMapLayer("res/tileset_building.png")),
unit_layer(OnMapLayer("res/tileset_unit.png"))
{

    this->current_state = current_state;
    this->window = &window;
    //utilisation d'un unique ptr :
    //étapes: instancier un pointeur, puis lui attribuer le "unique_ptr".get()
    shared_ptr<state::Map> current_map = (this->current_state->getMap());

    //this->game_object_list = move(current_state.getMap().get()->getListGameObject());
    int tmp_x = 0;
    int tmp_y = 0;
    current_map.get()->getSize(tmp_x,tmp_y);

    this->map_size_x = (unsigned int) tmp_x;
    this->map_size_y = (unsigned int) tmp_y;

}


bool DrawManager::updateState(shared_ptr<state::State> new_state) {
    this->current_state = new_state;
    window->clear(sf::Color::Black);

    setTerrain();
    setBuilding();
    setUnit();

    window->display();

    return true;
}


bool DrawManager::setTerrain ()
{
    shared_ptr<state::Map> current_map = this->current_state->getMap();
    drawer.initQuads(this->map_size_x+this->map_size_x*this->map_size_y);
    std::vector<DrawElement> elem;
    for(unsigned int x = 0; x < this->map_size_x;x ++)
    {
        for(unsigned int y=0; y< this->map_size_y;y++)
        {/*
           ///state::Terrain * local_terrain = current_map.get()->getTerrain(x,y).get();
          ///  drawer.setSpriteLocation(x+y*map_size_x,x,y);
         ///   //std::cout<<"adding "<<local_terrain->getTerrainType()<<" as "<<local_terrain->getTerrainType()<<std::endl;
        ///drawer.setSpriteTexture(0,local_terrain->getTerrainType(),x+y*map_size_x);
       ///*/
        state::Terrain * local_terrain = current_map.get()->getTerrain(x,y).get();
        elem.push_back(DrawElement(state::Position(x,y),local_terrain->getTerrainType()));
        }
    }
    drawer.draw(*window,state_sfml);
    return true;
}

bool DrawManager::setUnit ()
{
    state::Map * current_map = this->current_state->getMap().get();

    std::vector<state::Unit *> units_go;
    state::Position pos;
    for (unsigned int i = 0; i< current_map->getListGameObject().size();i++)
    {

        if(!current_map->getListGameObject()[i].get()->getProperty()->isStatic())
        {
            //cast en pointeur le unique pointeur => on peut utiliser les méthodes de la classe
            units_go.push_back((state::Unit*)current_map->getListGameObject()[i].get());
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

    state::Map * current_map = this->current_state->getMap().get();
    std::vector<state::Building *> build_go;
    state::Position pos;
    for (unsigned int i = 0; i< current_map->getListGameObject().size();i++)
    {
        if(current_map->getListGameObject()[i].get()->getProperty()->isStatic())
        {
            //cast en pointeur de building l'unique pointeur pour pouvoir utiliser les méthodes de building
            build_go.push_back((state::Building*)current_map->getListGameObject()[i].get());
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

bool DrawManager::stateChanged(const state::Event &event) {
    state::EventTypeId this_event = event.getEventType();
    std::cout << "event : "<< this_event<<std::endl;

    updateState(current_state);
    return true;
}