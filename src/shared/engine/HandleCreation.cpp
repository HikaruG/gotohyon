//
// Created by hikaru on 07/11/18.
//

#include <iostream>
#include "HandleCreation.h"

using namespace engine;
using namespace std;
using namespace state;

bool collisionHandler(State &state, unsigned int pos_x, unsigned int pos_y, bool is_static){
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
            if(is_static) {
                vector<shared_ptr<state::Building>> ally_buildings = list_player[i].get()->getPlayerBuildingList();
                for (int i = 0; i < (int) ally_buildings.size(); i++) {
                    if (ally_buildings[i].get()->getPosition() == position) {
                        cout << "cannot build there, already a building here !" << endl;
                        return false;
                    }
                }
            }
            else {
                vector<shared_ptr<state::Unit>> ally_units = list_player[i].get()->getPlayerUnitList();

                for (int i = 0; i < (int) ally_units.size(); i++) {
                    if (ally_units[i].get()->getPosition() == position) {
                        cout << "cannot create unit there; respect your allies !" << endl;
                        return false;
                    }
                }
            }
        }

    }
    return true;
    //fin gestion de la collision d'objets

}


CommandTypeId HandleCreation::getTypeId() const {
    return CommandTypeId::HANDLE_CREATION;
}


    /*ce que je compte faire : pour Ben
    Mettre en place un moyen de déterminer le type et le is_static lors du gestionnaire du clic.

 rajouter un attribut type dans un render pour enlever tous les paramètres des commandes pour laisser le state seulement.
 Lors du premier clic, si l'on choisit un batiment, le deuxieme clic nous donne le choix entre les différents unités que l'on peut créer (avec
 un affichage des choix, si possible) is_static = false à coup sûr

 Lors du premier clic, si l'on choisit un villageois, le deuxième clic est un mouvement; le 3ème sera alors le gestionnaire de création de batiments,
 avec les différents choix de batiments à construire, avec un affichage des choix si possible is_static = true à coup sur
    */

bool HandleCreation::execute(state::State &state, unsigned int pos_x, unsigned int pos_y, int type, bool is_static) {
    //vérifie si ce n'est pas une demmande de construction vide (pour les AI)
    if(type == -1)
        return false;

    Property farmer = Property("farmer",10,10,80,false,false,1);
    Property infantry = Property("infantry",10,20,120,false,false,1);
    Property archer = Property("archer",10,1,100,false,false,3);

    Property mine = Property("mine",10,10,50,true,false,0);
    Property farm = Property("farm",10,10,50,true,false,0);
    Property turret = Property("turret",10,10,150,true,false,0);
    Property town = Property("town",10,10,400,true,false,0);
    Property barrack = Property("barrack",10,10,200,true,false,0);

    std::vector<Property> buildings = {mine,farm,turret,town, barrack};
    std::vector<Property> units = {farmer,archer,infantry};
    unsigned int mvt_range= 0;

    unsigned int buildings_limit = 6;
    unsigned int units_limit = 5;
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
            if(collisionHandler(state, pos_x,pos_y,is_static)) {
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
            else {
                cout << "couldn't build here" << endl;
                return false;
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
                mvt_range = 1;
                req_gold = 75;
                req_food = 75;
                debug_info = "farmer";
                break;
            case state::archer:
                mvt_range = 3;
                req_gold = 100;
                req_food = 140;
                debug_info = "archer";
                break;
            case state::infantry:
                mvt_range =2;
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
            if(collisionHandler(state, pos_x,pos_y, is_static)) {
                //Unit::Unit (unsigned int movement_range, unsigned int gameobject_id, unsigned int player_id, state::Position pos, state::Property property, UnitType unit_type)
                shared_ptr<state::Unit> new_unit(new Unit(mvt_range,
                                                          (unsigned int) state.getMap().get()->getListGameObject().size(),
                                                          current_player_id, position,
                                                          units[type],
                                                          (state::UnitType) type));
                state.addUnit(move(new_unit));
                cout << "created new unit : " << debug_info << endl;
                state.getCurrentPlayer().get()->setRessource(-req_gold, -req_food);
            }
            else {
                cout << "couldn't build here" << endl;
                return false;
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













