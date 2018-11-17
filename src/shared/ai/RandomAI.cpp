//
// Created by hikaru on 13/11/18.
//

#include <state/Player.h>
#include <engine/Command.h>
#include "RandomAI.h"
#include "engine.h"
#include <iostream>

using namespace ai;
using namespace state;
using namespace std;

RandomAI::RandomAI(int random_seed) {
    std::mt19937 randgen(random_seed);
}

RandomAI::~RandomAI() = default;

bool RandomAI::run(engine::Engine &engine, state::Player& player, state::State& state) {
    //récupération de la map
    state::Map * map;
    unsigned int current_player = state.getCurrentPlayerId();

    //récupération de la liste des bâtiments du pc
    vector<shared_ptr<Building>> list_building = state.getCurrentPlayer(current_player)->getPlayerBuildingList();
    //std::cout<<"size for player "<<player.getPlayerId()<<" "<<list_building->size()<<std::endl;

    //récupération de la liste des unités du pc
    vector<shared_ptr<Unit>> list_unit = state.getCurrentPlayer(current_player)->getPlayerUnitList();


    //std::cout<<"size for player "<<player.getPlayerId()<<" "<<list_unit->size()<<std::endl;


    unsigned int old_x, old_y, new_x = 0, new_y = 0, distance;

    engine::HandleMovement commande_movement = engine::HandleMovement();
    engine::HandleCanAttack commande_canattack = engine::HandleCanAttack();
    engine::HandleDamage commande_damage = engine::HandleDamage();

    vector<shared_ptr<GameObject>> ennemy_objects;



    /***  implémentation des déplacements et des attaques aléatoires  ***/

    for(int i= 0; i < list_unit.size(); i++)
    {
        Unit * unit_i = list_unit[i].get();
        state::Position position_unit = unit_i->getPosition();
        old_y = position_unit.getY();
        old_x = position_unit.getX();
        distance = unit_i->getMovementRange();
        std::uniform_int_distribution<int> dis_x(-distance,distance);
        new_x = old_x + dis_x(randgen);

        std::uniform_int_distribution<int> dis_y(-abs(distance - new_x),abs(distance - new_x));
        new_y = old_y + dis_y(randgen);
        commande_movement.execute(*unit_i, state, new_x, new_y);

        //si l'unité peut attaquer, il attaque

        if(commande_canattack.execute(* unit_i, state, ennemy_objects)){

            int size_ennemy_list = ennemy_objects.size();
            std::uniform_int_distribution<int> dis_i(0,size_ennemy_list - 1);
            int i_object = dis_i(randgen);

            //identification de la position de l'objet choisi aléatoirement
            state::Position position_ennemy = ennemy_objects[i_object]->getPosition();
            //terrain sur lequel l'object est situé

            state::Terrain * object_terrain = map->getTerrain(position_ennemy.getX(),position_ennemy.getY()).get();
            commande_damage.execute(* unit_i, * ennemy_objects[i_object].get(),* object_terrain);
        }
    }

    /***  implémentation des créations d'unités   ***/







    return true;
}



