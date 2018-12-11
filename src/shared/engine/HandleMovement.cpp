//
// Created by hikaru on 06/11/18.
//

#include "HandleMovement.h"
#include <iostream>

using namespace engine;
using namespace std;

//HandleMovement::~HandleMovement() {}

CommandTypeId HandleMovement::getTypeId() const {
    return CommandTypeId::HANDLE_MOVEMENT;
}

bool HandleMovement::execute(state::State& state) {

    state::Position selected_position = state.getSelPosition();
    unsigned int new_x = selected_position.getX();
    unsigned int new_y = selected_position.getY();

    //récupération de la taille de la carte
    int max_x, max_y;
    state.getMap().get()->getSize(max_x,max_y);

    //si le mouvement demandé respect la distance de déplacement autorisé à l'unité
    if( (int)(state.getSelUnit().getPosition().getX()  + state.getSelUnit().getPosition().getY() - abs(((int)new_x + (int)new_y)) ) <= state.getSelUnit().getMovementRange() ){

        //si l'objet ne sort pas de la carte
        if((int) new_x >= 0 && (int) new_x < max_x && (int)new_y >= 0 && (int)new_y < max_y ) {
            //nouvelle position de l'objet
            state::Position position(new_x, new_y);

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
                        // si une unité est déjà présente, le déplacement devient impossible
                        if(ennemy_buildings[i].get()->getPosition() == position){
                            cout << "cannot move there; destroy the building first !" << endl;
                            return false;
                        }
                    };

                    //vérifie si il n'y a pas un batiment ennemi déjà présent à cette position
                    for(int i = 0; i < (int)ennemy_units.size(); i++){
                        //si un batiment est déjà présent, le déplacement devient impossible
                        if(ennemy_units[i].get()->getPosition() == position){
                            cout << "cannot move there; kill the unit first !" << endl;
                            return false;
                        }
                    };
                }

                //si le joueur dans la liste list_player est le joueur actuel
                else{
                    vector<shared_ptr<state::Unit>>ally_units = list_player[i].get()->getPlayerUnitList();

                    for(int i =0; i < (int)ally_units.size(); i++){
                        if(ally_units[i].get()->getPosition() == position){
                            cout << "cannot move there; respect your allies !" << endl;
                            return false;
                        }
                    }
                }

            }
            //fin gestion de la collision d'objets
            size_t terrain_type = state.getMap().get()->getTerrain(new_x,new_y).get()->getTerrainType();
            if(terrain_type == state::water){
                std::cout << "wow wow you can't go there !" << std::endl;
                return false;
            }

            if (state.getMap().get()->moveGameObject(state.getSelUnit().getGame_object_id(), position)) {
                //std::cout << "successfully moved" << std::endl;
            }
        }
        //l'objet se retrouvera en dehors de la carte
        else{
            //std::cout << "wow wow you can't go there !" << std::endl;
            return false;
        }
    }
    //la distance de déplacement est supérieur à la distance parcourable par l'unité
    else{
        std::cout << " you are not that fast... " << std::endl;
    }

    return true;
}


HandleMovement::~HandleMovement() {

}

HandleMovement::HandleMovement() {

}
