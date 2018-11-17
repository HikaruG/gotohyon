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

    Property farmer = Property("unit_farmer",10,10,100,false,3);
    Property infantry = Property("unit_infantry",10,10,100,false);
    Property archer = Property("unit_archer",10,10,100,false);

    Property mine = Property("building_mine",10,10,100,true);
    Property farm = Property("building_farm",10,10,100,true);
    Property turret = Property("building_turret",10,10,100,true);
    Property town = Property("building_town",10,10,100,true);
    Property barrack = Property("building_barrack",10,10,100,true);

    std::vector<Property> buildings = {mine,farm,turret,town, barrack};
    std::vector<Property> units = {farmer,infantry, archer};

    unsigned int buildings_limit = 5;
    unsigned int units_limit = 10;
    unsigned int current_player_id = state.getCurrentPlayerId();
    //all_objects_count = 0;

    state::Position position(pos_x, pos_y);

    state::Map * map = state.getMap().get();
    state::Terrain * terrain = map->getTerrain(pos_x,pos_y).get();

    if(terrain->getTerrainType() == state::water){
        cout << "Building cannot be created" << endl;
        return true;
    }

    if(is_static){
        if(type > 5) //il n' y a que 5 batiments
        {
            throw invalid_argument(" can't find the building ! ");
        }
        if(state.getCurrentPlayer(current_player_id).get()->getPlayerBuildingList().size() > buildings_limit){
            throw invalid_argument(" can't build anymore " );
        }
        //Building::Building (unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property prop, state::BuildingType build_type)
        shared_ptr<state::Building> new_building (new Building((unsigned int)state.getMap().get()->getListGameObject().size(),
                                                    current_player_id,
                                                    position,
                                                    buildings[type],
                                                    (state::BuildingType)type));
        state.addBuilding(position,new_building );

    }
    else{
        if(type > 3)
        {
            cout << "can't find the unit !" << endl;
            return true;
        }
        if(state.getCurrentPlayer(current_player_id).get()->getPlayerUnitList().size() > units_limit){
            throw invalid_argument(" can't create units anymore " );
        }
        //Unit::Unit (unsigned int movement_range, unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property property, UnitType unit_type)
        shared_ptr<state::Unit> new_unit (new Unit(  1,
                                                    (unsigned int)state.getMap().get()->getListGameObject().size(),
                                                     current_player_id,position,
                                                     units[type],
                                                     (state::UnitType)type));
        state.addUnit(position,new_unit );
    }
    return true;
}


bool HandleCreation::execute (state::State& state){
    return true;
}
HandleCreation::HandleCreation() = default;

HandleCreation::~HandleCreation () = default;













