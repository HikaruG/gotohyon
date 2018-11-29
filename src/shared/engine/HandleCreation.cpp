//
// Created by hikaru on 07/11/18.
//

#include <iostream>
#include "HandleCreation.h"

using namespace engine;
using namespace std;
using namespace state;

CommandTypeId HandleCreation::getTypeId() const {
    return CommandTypeId::HANDLE_CREATION;
}

bool HandleCreation::execute(state::State &state, unsigned int pos_x, unsigned int pos_y, int type, bool is_static) {

    Property farmer = Property("unit_farmer",10,10,100,false,false,1);
    Property infantry = Property("unit_infantry",10,10,100,false,false,1);
    Property archer = Property("unit_archer",10,10,100,false,false,3);

    Property mine = Property("building_mine",10,10,100,true,false,0);
    Property farm = Property("building_farm",10,10,100,true,false,0);
    Property turret = Property("building_turret",10,10,100,true,false,0);
    Property town = Property("building_town",10,10,100,true,false,0);
    Property barrack = Property("building_barrack",10,10,100,true,false,0);

    std::vector<Property> buildings = {mine,farm,turret,town, barrack};
    std::vector<Property> units = {farmer,archer,infantry};

    unsigned int buildings_limit = 5;
    unsigned int units_limit = 3;
    unsigned int current_player_id = state.getCurrentPlayerId();
    unsigned int my_gold = 0, my_food = 0, req_gold, req_food;
    state.getCurrentPlayer().get()->getRessource(my_food, my_gold);
    //all_objects_count = 0;

    state::Position position(pos_x, pos_y);

    state::Map * map = state.getMap().get();
    state::Terrain * terrain = map->getTerrain(pos_x,pos_y).get();

    if(terrain->getTerrainType() == state::water || terrain->getTerrainType() == mountain){
        cout << "Building cannot be created" << endl;
        return false;
    }

    if(is_static){
        if(type > 5) //il n' y a que 5 batiments
        {
            cout<< " can't find the building ! " << endl;
            return false;
        }
        if(state.getCurrentPlayer().get()->getPlayerBuildingList().size() > buildings_limit){
            //cout << " can't build more buildings !" <<endl;
            return false;
        }

        //selon le type de construction, les ressources nécéssaires varient
        switch(type){
            case state::town:
                req_gold = 600;
                req_food = 600;
                break;
            case state::farm:
                req_gold = 400;
                req_food = 200;
                break;
            case state::mine:
                req_gold = 200;
                req_food = 400;
                break;
            case state::barrack:
                req_gold = 400;
                req_food = 400;
                break;
            case state::turret:
                req_gold = 500;
                req_food = 500;
                break;
            default:
                cout << "wow wow, unknown building type" <<endl;
                return false;
        }
        if(my_food > req_food && my_gold > req_food) {
            //Building::Building (unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property prop, state::BuildingType build_type)
            shared_ptr<state::Building> new_building(
                    new Building((unsigned int) state.getMap().get()->getListGameObject().size(),
                                 current_player_id,
                                 position,
                                 buildings[type],
                                 (state::BuildingType) type));
            state.addBuilding(move(new_building));
            my_food -= req_food;
            my_gold -= req_gold;
        }

    }
    else{
        if(type > 3)
        {
            cout << "can't find the unit !" << endl;
            return false;
        }
        if(state.getCurrentPlayer().get()->getPlayerUnitList().size() > units_limit){
           // cout << "can't create more units; go fight someone already !" << endl;
        }

        switch(type){
            case state::farmer:
                req_gold = 75;
                req_food = 75;
                break;
            case state::archer:
                req_gold = 100;
                req_food = 140;
                break;
            case state::infantry:
                req_gold = 140;
                req_food = 100;
                break;
            default:
                cout << "wow wow, unknown unit type" <<endl;
                return false;
        }
        if(my_food > req_food && my_gold > req_food) {
            //Unit::Unit (unsigned int movement_range, unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property property, UnitType unit_type)
            shared_ptr<state::Unit> new_unit(new Unit(1,
                                                      (unsigned int) state.getMap().get()->getListGameObject().size(),
                                                      current_player_id, position,
                                                      units[type],
                                                      (state::UnitType) type));
            state.addUnit(move(new_unit));
            my_food -= req_food;
            my_gold -= req_gold;
        }
    }
    return true;
}


bool HandleCreation::execute (state::State& state){
    return true;
}
HandleCreation::HandleCreation() = default;

HandleCreation::~HandleCreation () = default;













