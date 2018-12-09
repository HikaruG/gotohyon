//
// Created by hikaru on 07/11/18.
//

#include <iostream>
#include "HandleCreation.h"

using namespace engine;
using namespace std;
using namespace state;

bool collisionHandler(State &state, unsigned int pos_x, unsigned int pos_y){
    Position position = Position(pos_x,pos_y);
    //début gestion de la collision d'objets
    vector<shared_ptr<state::Player>> list_player = state.getListPlayer();
    for (int i = 0; i < (int)list_player.size(); i++){

        //si le joueur dans la liste list_player n'est pas le joueur actuel
        if(list_player[i].get()->getPlayerId() != state.getCurrentPlayerId()){
            //récupère les listes des batiments et des unités des ennemis
            vector<shared_ptr<state::Unit>>ennemy_units = list_player[i].get()->getPlayerUnitList();
            vector<shared_ptr<state::Building>> ennemy_buildings = list_player[i].get()->getPlayerBuildingList();

            //vérifie si il y a pas une unité ennemie déja présente à cette position
            for(int i = 0; i < (int)ennemy_buildings.size(); i++){
                // si une unité est déjà présente, la création devient impossible
                if(ennemy_buildings[i].get()->getPosition() == position){
                    cout << "cannot create there; destroy the ennemy's building first !" << endl;
                    return false;
                }
            };

            //vérifie si il n'y a pas un batiment ennemi déjà présent à cette position
            for(int i = 0; i < (int)ennemy_units.size(); i++){
                //si un batiment est déjà présent, la création devient impossible
                if(ennemy_units[i].get()->getPosition() == position){
                    cout << "cannot create there; kill the unit first !" << endl;
                    return false;
                }
            };
        }

            //si le joueur dans la liste list_player est le joueur actuel
        else{
            vector<shared_ptr<state::Unit>>ally_units = list_player[i].get()->getPlayerUnitList();

            for(int i =0; i < (int)ally_units.size(); i++){
                if(ally_units[i].get()->getPosition() == position){
                    cout << "cannot create there; respect your allies !" << endl;
                    return false;
                }
            }
        }

    }
    //fin gestion de la collision d'objets

}

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

    string debug_info = "none";
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
                debug_info = "town";
                break;
            case state::farm:
                req_gold = 400;
                req_food = 200;
                debug_info = "farm";
                break;
            case state::mine:
                req_gold = 200;
                req_food = 400;
                debug_info = "mine";
                break;
            case state::barrack:
                req_gold = 400;
                req_food = 400;
                debug_info = "barrack";
                break;
            case state::turret:
                req_gold = 500;
                req_food = 500;
                debug_info = "turret";
                break;
            default:
                cout << "wow wow, unknown building type" <<endl;
                return false;
        }
        if(my_food > req_food && my_gold > req_food) {
            //vérifie si la case est disponible pour la création
            if(collisionHandler(state, pos_x,pos_y)) {
                //Building::Building (unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property prop, state::BuildingType build_type)
                shared_ptr<state::Building> new_building(
                        new Building((unsigned int) state.getMap().get()->getListGameObject().size(),
                                     current_player_id,
                                     position,
                                     buildings[type],
                                     (state::BuildingType) type));
                state.addBuilding(move(new_building));
                state.getCurrentPlayer().get()->setRessource(-req_gold, -req_food);
                cout << "created new building : " << debug_info << endl;
            }
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
                debug_info = "farmer";
                break;
            case state::archer:
                req_gold = 100;
                req_food = 140;
                debug_info = "archer";
                break;
            case state::infantry:
                req_gold = 140;
                req_food = 100;
                debug_info = "infantry";
                break;
            default:
                cout << "wow wow, unknown unit type" <<endl;
                return false;
        }
        if(my_food > req_food && my_gold > req_food) {
            //vérifie si la case est disponible pour la création
            if(collisionHandler(state, pos_x,pos_y)) {
                //Unit::Unit (unsigned int movement_range, unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property property, UnitType unit_type)
                shared_ptr<state::Unit> new_unit(new Unit(1,
                                                          (unsigned int) state.getMap().get()->getListGameObject().size(),
                                                          current_player_id, position,
                                                          units[type],
                                                          (state::UnitType) type));
                state.addUnit(move(new_unit));
                cout << "created new unit : " << debug_info << endl;
                state.getCurrentPlayer().get()->setRessource(-req_gold, -req_food);
            }
        }
    }
    return true;
}


bool HandleCreation::execute (state::State& state){
    return true;
}
HandleCreation::HandleCreation() = default;

HandleCreation::~HandleCreation () = default;













