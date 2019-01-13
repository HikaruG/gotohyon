//
// Created by hikaru on 06/11/18.
//

#include "HandleCanAttack.h"
#include <iostream>

using namespace engine;
using namespace std;
using namespace state;


HandleCanAttack::HandleCanAttack() = default;

HandleCanAttack::HandleCanAttack(shared_ptr<state::Unit> selected_unit) {
    this->selected_unit = selected_unit;
}


HandleCanAttack::~HandleCanAttack() = default;



CommandTypeId HandleCanAttack::getTypeId() const{
    return CommandTypeId::HANDLE_CANATTACK;
}


bool HandleCanAttack::execute(state::State& state ) {
    // permet de réinitialiser la liste d'objets à 0
    state.resetInRange();

    //current_x et current_y sont les coordonnées de l'unité considéré
    int current_x, current_y;
    current_x = this->selected_unit->getPosition().getX();
    current_y = this->selected_unit->getPosition().getY();

    //others_x et others_y sont les coordonnées des autres unités présentent sur la carte
     int others_x, others_y;
    int distance = 0;
    bool canattack = false;

    vector<shared_ptr<GameObject>> list_ennemies_object = {};
    vector<shared_ptr<GameObject>> in_range_list = {};

    // liste des unités/bâtiments des ennemis
    for(int i =0;i < (int)state.getMap().get()->getListGameObject().size(); i++){

        //les objets avec un player_id différent de celui de l'unité "current_unit"
        if(state.getMap().get()->getListGameObject()[i].get()->getPlayerId() != this->selected_unit->getPlayerId()){
            list_ennemies_object.push_back(state.getMap().get()->getListGameObject()[i]);
        }
    }

    Terrain * terrain = state.getMap().get()->getTerrain(current_x,current_y).get();

    //cas de l'archer
    if (this->selected_unit->getUnitType() == state::archer) {
        switch (terrain->getTerrainType()) {
            case state::forest:
                distance = 2; //la foret donne un malus de -1 en vision, ainsi la range est diminuée de 1.
                break;
            default:
                distance = 3; //la range par défaut de l'archer est de 3.
        }
        for (shared_ptr<GameObject >s : list_ennemies_object) {
            others_x = s->getPosition().getX();
            others_y = s->getPosition().getY();
            if (std::abs(current_x + current_y - (others_x + others_y)) < distance) {
                canattack = true; //si une unité ennemie se trouve dans un rayon de "distance" ou moins, l'archer peut attaquer
                in_range_list.push_back(s);
            }
        }
    }

    //cas des autres unités cac
    else {
        for (shared_ptr<GameObject >s : list_ennemies_object) {
            others_x = s->getPosition().getX();
            others_y = s->getPosition().getY();
            if (others_y == current_y - 1 || others_y == current_y + 1) {//cible ennemie en haut ou en bas de l'unité considéré
                if (others_x == current_x) {
                    canattack = true;
                    in_range_list.push_back(s);
                }
            } else if (others_x == current_x - 1 || others_x == current_x + 1) {//cible ennemie à gauche u à droite de l'unité considéré
                if (others_y == current_y) {
                    canattack = true;
                    in_range_list.push_back(s);
                }
            }
        }
    }
    state.setInRange(in_range_list);
    if(canattack){
        return true;
    }
    //si l'unité ne peut pas attaquer, elle devient indisponible
    if(this->selected_unit->getUnitType()!=farmer)
        this->selected_unit->getProperty()->setAvailability(false);
    return false;
}


bool HandleCanAttack::undo(state::State &state) {
    return true;
}


void HandleCanAttack::serialize (Json::Value& out) const{
    out["CommandId"]=5;
    out["select_unit_id"]=this->selected_unit.get()->getGame_object_id();
}

HandleCanAttack* HandleCanAttack::deserialize (Json::Value& out){
    this->selected_unit=nullptr;
    this->selected_unit_id = out.get("selected_unit_id",0).asUInt();
}