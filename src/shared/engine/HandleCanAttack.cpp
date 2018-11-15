//
// Created by hikaru on 06/11/18.
//

#include "HandleCanAttack.h"

using namespace engine;


CommandTypeId HandleCanAttack::getTypeId() const{
    return CommandTypeId::HANDLE_ATTACK;
}

bool HandleCanAttack::execute(state::Unit &current_unit, state::State& state, std::vector<state::GameObject *> * in_range_list) {

    unsigned int current_x, current_y;
    current_unit.getPosition().getPosition(&current_x,
                                           &current_y); //current_x et current_y sont les coordonnées de l'unité considéré

    unsigned int others_x; //others_x et others_y sont les coordonnées des autres unités présentent sur la carte
    unsigned int others_y;
    int distance = 0;
    bool canattack = false;
    //in_range_list = nullptr;
    std::vector<state::Player *> * ennemy_list = new std::vector<state::Player *>;
    std::vector<state::Unit *> * ennemy_unit_list = new std::vector<state::Unit *>;
    std::vector<state::Building *> * ennemy_building_list = new std::vector<state::Building *>;

    // liste des unités/bâtiments des ennemis
    for(state::Player* s : *ennemy_list)
        if(s->getPlayerId() != current_unit.getPlayerId()){
            s->getPlayerBuildingList(* ennemy_building_list);
            s->getPlayerUnitList(* ennemy_unit_list);
        }

    state::Terrain terrain;
    state.getMap()->getTerrain(current_x,current_y,&terrain);

    //cas de l'archer
    if (current_unit.getUnitType() == state::archer) {
        switch (terrain.getTerrainType()) {
            case state::forest:
                distance = 2; //la foret donne un malus de -1 en vision, ainsi la range est diminuée de 1.
                break;
            default:
                distance = 3; //la range par défaut de l'archer est de 3.
        }
        for (state::Unit *s : *ennemy_unit_list) {
            s->getPosition().getPosition(&others_x, &others_y);
            if (abs(current_x + current_y - (others_x + others_y)) < distance) {
                canattack = true; //si une unité ennemie se trouve dans un rayon de "distance" ou moins, l'archer peut attaquer
                in_range_list->push_back((state::GameObject *)s);
            }
        }
        for (state::Building *s : *ennemy_building_list) {
            s->getPosition().getPosition(&others_x, &others_y);
            if (abs(current_x + current_y - (others_x + others_y)) < distance) {
                canattack = true; //si un batiment ennemi se trouve dans un rayon de "distance" ou moins, l'archer peut attaquer
                in_range_list->push_back((state::GameObject *)s);
            }
        }
    }

    //cas des autres unités cac
    else {
        //cas pour les unités ennemies
        for (state::Unit *s: *ennemy_unit_list) {
            s->getPosition().getPosition(&others_x, &others_y);
            if (others_y == current_y - 1 || others_y == current_y + 1) {//cible ennemie en haut ou en bas de l'unité considéré
                if (others_x == current_x) {
                    canattack = true;
                    in_range_list->push_back((state::GameObject *)s);
                }
            } else if (others_x == current_x - 1 || others_x == current_x + 1) {//cible ennemie à gauche u à droite de l'unité considéré
                if (others_y == current_y) {
                    canattack = true;
                    in_range_list->push_back((state::GameObject *)s);
                }
            }
        }

        //cas pour les batiments ennemis
        for (state::Building *s: *ennemy_building_list) {
            s->getPosition().getPosition(&others_x, &others_y);
            if (others_y == current_y - 1 || others_y == current_y + 1) {//cible ennemie en haut ou en bas de l'unité considéré
                if (others_x == current_x) {
                    canattack = true;
                    in_range_list->push_back((state::GameObject *)s);
                }
            } else if (others_x == current_x - 1 || others_x == current_x + 1) {//cible ennemie à gauche u à droite de l'unité considéré
                if (others_y == current_y) {
                    canattack = true;
                    in_range_list->push_back((state::GameObject *)s);
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

