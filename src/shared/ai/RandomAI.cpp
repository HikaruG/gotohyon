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

bool RandomAI::run(engine::Engine &engine, state::State& state) {



    //récupération de la liste des bâtiments du pc
    vector<shared_ptr<Building>> list_building = state.getCurrentPlayer()->getPlayerBuildingList();


    //récupération de la liste des unités du pc
    vector<shared_ptr<Unit>> list_unit = state.getCurrentPlayer()->getPlayerUnitList();


    //variables pour déterminer la nouvelle position de l'unité
    int old_x, old_y, new_x = 0, new_y = 0, distance_x, distance_y;

    engine::HandleMovement commande_movement = engine::HandleMovement();
    engine::HandleCanAttack commande_canattack = engine::HandleCanAttack();
    engine::HandleDamage commande_damage = engine::HandleDamage();
    engine::HandleTurn commande_turn = engine::HandleTurn();

    //liste de game_object ennemie
    vector<shared_ptr<GameObject>> ennemy_objects;
    //indice de l'ennemi choisi comme cible lors de l'attaque
    int i_object = 0;




    /***  implémentation des déplacements et des attaques aléatoires  ***/

    for(int i= 0; i < (int)list_unit.size(); i++)
    {
        Unit * unit_i = list_unit[i].get();
        state::Position position_unit = unit_i->getPosition();
        old_y = (int)position_unit.getY();
        old_x = (int)position_unit.getX();
        distance_x = unit_i->getMovementRange();
        std::uniform_int_distribution<int> dis_x(-distance_x,distance_x);
        new_x = old_x + dis_x(randgen);

        distance_y = distance_x - abs(old_x - new_x);


        std::uniform_int_distribution<int> dis_y(-distance_y,distance_y);
        new_y = old_y + dis_y(randgen);
        commande_movement.execute(*unit_i, state, new_x, new_y);

        //si l'unité peut attaquer, il attaque

        if(commande_canattack.execute(* unit_i, state, ennemy_objects)){

            int size_ennemy_list = ennemy_objects.size();
            std::uniform_int_distribution<int> dis_i(0,size_ennemy_list - 1);
            i_object = dis_i(randgen);
            cout << "la cible ennemie choisie est la " << i_object << endl;

            //identification de la position de l'objet choisi aléatoirement
            state::Position position_ennemy = ennemy_objects[i_object]->getPosition();
            cout << "la cible ennemie se situe à X: " << ennemy_objects[i_object]->getPosition().getX() << endl;
            cout << "\nla cible ennemie se situe à Y: " << ennemy_objects[i_object]->getPosition().getY() << endl;


            //terrain sur lequel l'object est situé
            state::Terrain * object_terrain = state.getMap().get()->getTerrain(position_ennemy.getX(),position_ennemy.getY()).get();
            commande_damage.execute(state,unit_i, ennemy_objects[i_object].get(),* object_terrain);
        }
    }

    /***  implémentation des créations d'unités   ***/

    //commande de fin de tour; préparation pour le joueur suivant
    commande_turn.execute(state);





    return true;
}



