//
// Created by ben on 25/10/18.
//
#include "DrawManager.h"
#include "state.h"
#include "render.h"
#include <iostream>
using namespace render;
using namespace std;


DrawManager::DrawManager ( shared_ptr<state::State> current_state, shared_ptr<sf::RenderWindow> window)
:terrain_layer(OnMapLayer("res/tileset_terrain.png")),building_layer(OnMapLayer("res/tileset_building.png")),
unit_layer(OnMapLayer("res/tileset_unit.png")),user_interact(PlayerAction(window))
{

    this->current_state = current_state;
    this->window = window;
    int tmp_x = 0;
    int tmp_y = 0;
    current_state.get()->getMap().get()->getSize(tmp_x,tmp_y);
    this->map_size_x = (unsigned int) tmp_x;
    this->map_size_y = (unsigned int) tmp_y;

    terrain_layer.setNext(&building_layer);
    building_layer.setNext(&unit_layer);

}


bool DrawManager::updateState(shared_ptr<state::State> new_state) {
    this->current_state = new_state;
    window.get()->clear(sf::Color::Black);



    terrain_layer.draw(*window,state_sfml);
    window->display();

    return true;
}

bool DrawManager::forceRefresh (std::shared_ptr<state::State> state)
{
    this->current_state = state;
    window.get()->clear(sf::Color::Black);
    setTerrain();
    setBuilding();
    setUnit();
    terrain_layer.draw(*window,state_sfml);
    window->display();

    return true;
}


bool DrawManager::setTerrain ()
{

    shared_ptr<state::Map> current_map = this->current_state->getMap();
    std::vector<DrawElement> elem;
    for(unsigned int x = 0; x < this->map_size_x;x ++) {
        for (unsigned int y = 0; y < this->map_size_y; y++) {
            shared_ptr<state::Terrain> local_terrain = current_map.get()->getTerrain(x, y);
            elem.push_back(DrawElement(state::Position(x, y), local_terrain.get()->getTerrainType()));
        }
    }

    terrain_layer.updateElements(elem);
    return true;
}

bool DrawManager::setUnit ()
{
    state::Map * current_map = this->current_state->getMap().get();


    state::Position pos;
    vector<DrawElement> elem;
    for (unsigned int i = 0; i< current_map->getListGameObject().size();i++)
    {

        if(!current_map->getListGameObject()[i].get()->getProperty()->isStatic())
        {
            //cast en pointeur le unique pointeur => on peut utiliser les méthodes de la classe
            state::Unit * units_go = (state::Unit *) current_map->getListGameObject()[i].get();
            elem.push_back(DrawElement(units_go->getPosition(),units_go->getUnitType()));
        }
    }

    unit_layer.updateElements(elem);
    return true;
}

bool DrawManager::setBuilding ()
{
    vector<DrawElement> elem;
    state::Map * current_map = this->current_state->getMap().get();
    state::Position pos;
    for (unsigned int i = 0; i< current_map->getListGameObject().size();i++) {
        if (current_map->getListGameObject()[i].get()->getProperty()->isStatic()) {
            //cast en pointeur de building l'unique pointeur pour pouvoir utiliser les méthodes de building
            state::Building * build_go = (state::Building *) current_map->getListGameObject()[i].get();
            elem.push_back(DrawElement(build_go->getPosition(), build_go->getBuildingType()));
        }

    }
    building_layer.updateElements(elem);
    return true;
}

bool DrawManager::stateChanged(const state::Event &event) {

    state::EventTypeId this_event = event.getEventType();
    std::cout << "event : "<< this_event<<std::endl;

    /*
     * events:
     * 0 Player changed
     * 1 Unit selected -- or other gui related stuuf
     * 2 terrain changed
     * 3 building changed
     * 4 unit changed
     * setTerrain();
    setBuilding();
    setUnit();
     */
    updateState(current_state);
    switch(this_event) {
        case 0 : break;
        case 1 : break;
        case 2 : setTerrain();
            break;
        case 3 :setBuilding();
            break;
        case 4 : setUnit();
            break;
        default: std::cout<<"invalid event from state received"<<endl;
    }


    //call the right to update

    return true;
}