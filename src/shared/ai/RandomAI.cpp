//
// Created by hikaru on 13/11/18.
//

#include <state/Player.h>
#include <engine/Command.h>
#include "RandomAI.h"
#include "engine.h"
#include <iostream>

using namespace ai;

RandomAI::RandomAI(int random_seed) {
    std::mt19937 randgen(random_seed);
}

RandomAI::~RandomAI() = default;

bool RandomAI::run(engine::Engine &engine, state::Player& player, state::State& state) {
    //récupération de la map
    state::Map * map;

    //récupération de la liste des bâtiments du pc
    std::vector<state::Building *> * list_building = new std::vector<state::Building *>;
    player.getPlayerBuildingList(* list_building);
    //std::cout<<"size for player "<<player.getPlayerId()<<" "<<list_building->size()<<std::endl;
    //récupération de la liste des unités du pc
    std::vector<state::Unit *> * list_unit = new std::vector<state::Unit *>;
    player.getPlayerUnitList(* list_unit);
    //std::cout<<"size for player "<<player.getPlayerId()<<" "<<list_unit->size()<<std::endl;


    unsigned int old_x, old_y, new_x = 0, new_y = 0, distance;

    engine::HandleMovement commande_movement = engine::HandleMovement();
    engine::HandleCanAttack commande_canattack = engine::HandleCanAttack();
    engine::HandleDamage commande_damage = engine::HandleDamage();


    //création d'une liste de GameObject attaquable
    std::vector<state::GameObject *> * ennemy_object_list;
    //taille de liste des objets attaquables
    int size_ennemy_list = 0;
    //position dans la liste du gameobject choisi aléatoirement
    int i_object = 0;


    /***  implémentation des déplacements et des attaques aléatoires  ***/

    for(state::Unit * s : * list_unit)
    {
        state::Position position_unit = s->getPosition();
        old_y = position_unit.getY();
        old_x = position_unit.getX();
        distance = s->getMovementRange();
        std::uniform_int_distribution<int> dis_x(-distance,distance);
        new_x = old_x + dis_x(randgen);

        std::uniform_int_distribution<int> dis_y(-abs(distance - new_x),abs(distance - new_x));
        new_y = old_y + dis_y(randgen);
        commande_movement.execute(*s, state, new_x, new_y);

        //si l'unité peut attaquer, il attaque
        if(commande_canattack.execute(* s, state, ennemy_object_list)){
            size_ennemy_list = ennemy_object_list->size();

            //rajouter la valeur aléatoire
            std::uniform_int_distribution<int> dis_i(0,size_ennemy_list - 1);
            i_object = dis_i(randgen);
            //fin du rajout à faire

            //identification de l'objet choisi aléatoirement
            state::GameObject * ennemy_object = ennemy_object_list->at(i_object);
            state::Position position_ennemy = ennemy_object->getPosition();
            //terrain sur lequel l'object est situé
            state::Terrain object_terrain;
            map->getTerrain(position_ennemy.getX(),position_ennemy.getY(),&object_terrain);
            commande_damage.execute(* s, * ennemy_object, object_terrain);
        }
    }


    /***  implémentation des créations d'unités   ***/


    return true;
}



