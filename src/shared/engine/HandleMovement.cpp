//
// Created by hikaru on 06/11/18.
//

#include "HandleMovement.h"
#include <iostream>

using namespace engine;
using namespace std;


HandleMovement::HandleMovement()=default;

HandleMovement::HandleMovement(int new_x, int new_y,
                               state::Unit* selected_unit) {
    this->old_x = selected_unit->getPosition().getX();
    this->old_y = selected_unit->getPosition().getY();
    this->new_x = (unsigned int) new_x;
    this->new_y = (unsigned int) new_y;
    this->selected_unit = selected_unit;
    this->selected_unit_id = selected_unit->getGame_object_id();
}

CommandTypeId HandleMovement::getTypeId() const {
    return CommandTypeId::HANDLE_MOVEMENT;
}

bool HandleMovement::execute(state::State& state) {

    if(this->new_x == (int)this->selected_unit->getPosition().getX() && this->new_y == (int)this->selected_unit->getPosition().getY()) {
        cout << this->selected_unit->getProperty()->getStringType() << " didn't move" << endl;
        return true;
    }
    int max_x, max_y;
    state.getMap().get()->getSize(max_x,max_y);

    //si le mouvement demandé respect la distance de déplacement autorisé à l'unité
    if( (int)(this->selected_unit->getPosition().getX()  + this->selected_unit->getPosition().getY() - abs(((int)this->new_x + (int)this->new_y)) ) <= this->selected_unit->getMovementRange() ){
        //si l'objet ne sort pas de la carte
        if(this->new_x >= 0 &&  this->new_x < max_x && this->new_y >= 0 && this->new_y < max_y ) {
            //nouvelle position de l'objet
            state::Position position(this->new_x, this->new_y);

            //début gestion de la collision d'objets
            vector<shared_ptr<state::Player>> list_player = state.getListPlayer();
            for (int i = 0; i < (int)list_player.size(); i++){
                //si le joueur dans la liste list_player n'est pas le joueur actuel
                if(list_player[i].get()->getPlayerId() != state.getCurrentPlayerId()){
                    //récupère les listes des batiments et des unités des ennemis
                    vector<shared_ptr<state::Unit>>ennemy_units = list_player[i].get()->getPlayerUnitList();
                    vector<shared_ptr<state::Building>> ennemy_buildings = list_player[i].get()->getPlayerBuildingList();
                    //vérifie si il y a pas une unité ennemie déja présente à cette position
                    for(int j = 0; j < (int)ennemy_buildings.size(); j++){
                        // si une unité est déjà présente, le déplacement devient impossible
                        if(ennemy_buildings[j].get()->getPosition() == position){
                            cout << this->selected_unit->getProperty()->getStringType() <<" cannot move there" << endl;
                            cout << "destroy " << ennemy_buildings[j].get()->getProperty()->getStringType() <<" first" << endl;
                            return false;
                        }
                    };
                    //vérifie si il n'y a pas un batiment ennemi déjà présent à cette position
                    for(int j = 0; j < (int)ennemy_units.size(); j++){
                        //si un batiment est déjà présent, le déplacement devient impossible
                        if(ennemy_units[j].get()->getPosition() == position){
                            cout << this->selected_unit->getProperty()->getStringType() <<" cannot move there" << endl;
                            cout << "destroy " << ennemy_units[j].get()->getProperty()->getStringType() <<" first" << endl;
                            return false;
                        }
                    };
                }
                //si le joueur dans la liste list_player est le joueur actuel
                else{
                    vector<shared_ptr<state::Unit>>ally_units = list_player[i].get()->getPlayerUnitList();

                    for(int j =0; j < (int)ally_units.size(); j++){
                        if(ally_units[j].get()->getPosition() == position){
                            cout << this->selected_unit->getProperty()->getStringType() <<" cannot move there" << endl;
                            cout << "respect " << ally_units[j].get()->getProperty()->getStringType() << endl;
                            return false;
                        }
                    }
                }
            }
            //fin gestion de la collision d'objets

            size_t terrain_type = state.getMap().get()->getTerrain(this->new_x,this->new_y).get()->getTerrainType();
            if(terrain_type == state::water){
                std::cout << "you can't go there, it's water" << std::endl;
                return false;
            }
            if (state.getMap().get()->moveGameObject(this->selected_unit->getGame_object_id(), position)){
                cout << " player" <<selected_unit->getPlayerId() << selected_unit->getProperty()->getStringType()
                     << " moved from " << old_x << " to the new position x = " << selected_unit->getPosition().getX() << endl;
                cout << " player" << selected_unit->getPlayerId() << selected_unit->getProperty()->getStringType()
                     << " moved from " << old_y << " to the new position y = " << new_y << endl;
            } else
                return false;
        }
        //l'objet se retrouvera en dehors de la carte
        else{
            std::cout << "you can't go there, it's out of the box !" << std::endl;
            return false;
        }
    }
    //la distance de déplacement est supérieur à la distance parcourable par l'unité
    else{
        std::cout << " you are not that fast " << std::endl;
        return false;
    }
    return true;
}

bool HandleMovement::undo(state::State &state) {
    if(!this->selected_unit)
    {
        this->selected_unit=(state::Unit*) state.getGameObject(this->selected_unit_id).get();
        if(!this->selected_unit)
        {
            cout<<"Fatal Error : no object with game id\n"<<this->selected_unit_id<<endl;
            exit(404);
        }
    }
    state.getMap()->moveGameObject(this->selected_unit_id,state::Position(this->old_x,this->old_y));
    return true;
}

void HandleMovement::serialize (Json::Value& out) const{
    out["CommandId"]=4;
    out["new_x"]=this->new_x;
    out["new_y"]=this->new_y;
    out["old_x"]=this->old_x;
    out["old_y"]=this->old_y;
    out["selected_unit_id"]=this->selected_unit->getGame_object_id();
}

HandleMovement* HandleMovement::deserialize (Json::Value& out){
    this->selected_unit= nullptr;
    this->new_x=out.get("new_x",0).asUInt();
    this->new_y=out.get("new_y",0).asUInt();
    this->old_x=out.get("old_x",0).asUInt();
    this->old_y=out.get("old_y",0).asUInt();
    this->selected_unit_id=out.get("selected_unit_id",0).asUInt();
    return this;
}


HandleMovement::~HandleMovement() {
}