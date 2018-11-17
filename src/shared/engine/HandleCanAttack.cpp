//
// Created by hikaru on 06/11/18.
//

#include "HandleCanAttack.h"

using namespace engine;
using namespace std;
using namespace state;

CommandTypeId HandleCanAttack::getTypeId() const{
    return CommandTypeId::HANDLE_ATTACK;
}

bool HandleCanAttack::execute(state::Unit& current_unit, state::State& state, vector<shared_ptr<GameObject>> list_in_range ) {

    //current_x et current_y sont les coordonnées de l'unité considéré
    int current_x, current_y;
    current_x = current_unit.getPosition().getX();
    current_y = current_unit.getPosition().getY();

    //others_x et others_y sont les coordonnées des autres unités présentent sur la carte
     int others_x, others_y;
    int distance = 0;
    bool canattack = false;
    //in_range_list = nullptr;

    vector<shared_ptr<GameObject>> list_ennemies_object;
    // liste des unités/bâtiments des ennemis
    for(int i =0;i < state.getMap().get()->getListGameObject().size(); i++){

        //les objets avec un player_id différent de celui de l'unité "current_unit"
        if(state.getMap().get()->getListGameObject()[i].get()->getPlayerId() != current_unit.getPlayerId()){
            list_ennemies_object.push_back(move(state.getMap().get()->getListGameObject()[i]));
        }
    }


    Terrain * terrain = state.getMap()->getTerrain(current_x,current_y).get();

    //cas de l'archer
    if (current_unit.getUnitType() == state::archer) {
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
                list_in_range.push_back(move(s));
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
                    list_in_range.push_back(move(s));
                }
            } else if (others_x == current_x - 1 || others_x == current_x + 1) {//cible ennemie à gauche u à droite de l'unité considéré
                if (others_y == current_y) {
                    canattack = true;
                    list_in_range.push_back(move(s));
                }
            }
        }
    }
    if(canattack){
        return true;
    }
    return false;
}

bool HandleCanAttack::execute (state::State &state) {
    return true;
}
HandleCanAttack::HandleCanAttack() = default;

HandleCanAttack::~HandleCanAttack() = default;

