//
// Created by hikaru on 06/11/18.
//

#include "HandleAttack.h"

using namespace engine;


CommandTypeId HandleAttack::getTypeId() const{
    return CommandTypeId::HANDLE_ATTACK;
}

bool HandleAttack::execute(state::Unit &current_unit,
                           state::Terrain &terrain, state::State& state, bool canattack, std::vector<state::GameObject *> * in_range_list) {
    unsigned int current_x, current_y;
    current_unit.getPosition().getPosition(&current_x,
                                           &current_y); //cuurent_x et current_y sont les coordonnées de l'unité considéré

    unsigned int others_x; //others_x et others_y sont les coordonnées des autres unités présentent sur la carte
    unsigned int others_y;
    int distance = 0;
    canattack = false;
    //in_range_list = nullptr;
    std::vector<state::GameObject *> ennemy_list;

    if (current_unit.getUnitType() == state::archer) {
        switch (terrain.getTerrainType()) {
            case state::forest:
                distance = 2; //la foret donne un malus de -1 en vision, ainsi la range est diminuée de 1.
                break;
            default:
                distance = 3; //la range par défaut de l'archer est de 3.
        }
        for (state::GameObject *s : ennemy_list) {
            s->getPosition().getPosition(&others_x, &others_y);
            if (abs(current_x + current_y - (others_x + others_y)) < distance) {
                canattack = true; //si une cible ennemie se trouve dans un rayon de "distance" ou moins, l'archer peut attaquer
                in_range_list->push_back(s);
            }
        }

    } else {
        for (state::GameObject *s: ennemy_list) {
            s->getPosition().getPosition(&others_x, &others_y);
            if (others_y == current_y - 1 || others_y == current_y + 1) {//cible ennemie en haut ou en bas de l'unité considéré
                if (others_x == current_x) {
                    canattack = true;
                    in_range_list->push_back(s);
                }
            } else if (others_x == current_x - 1 || others_x == current_x + 1) {//cible ennemie à gauche u à droite de l'unité considéré
                if (others_y == current_y) {
                    canattack = true;
                    in_range_list->push_back(s);
                }
            }
        }
    }
    return true;

}
bool HandleAttack::execute (state::State &state) {
    return true;
}
HandleAttack::HandleAttack() = default;

HandleAttack::~HandleAttack() = default;

