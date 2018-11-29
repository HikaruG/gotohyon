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

    engine::HandleGrowth commande_growth = engine::HandleGrowth();
    //rajout des ressources
    commande_growth.execute(state);


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
    engine::HandleCreation commande_create = engine::HandleCreation();

    //liste de game_object ennemie
    vector<shared_ptr<GameObject>> ennemy_objects;
    //indice de l'ennemi choisi comme cible lors de l'attaque
    int i_object = 0;

    //randgen de 1/2
    std::uniform_int_distribution<int> dis_half(0,1);
    int execute = dis_half(randgen);
    //randgen pour les différents batiments: -1 signifie le batiment vide
    std::uniform_int_distribution<int> dis_buildings(mine, state::maxBuilding - 1);
    int building_type = -1;


    for(int i= 0; i < (int)list_unit.size(); i++)
    {
        Unit * unit_i = list_unit[i].get();
        /***  implémentation des constructions de batiments aléatoires ***/
        if(unit_i->getUnitType()==farmer){}

        /***  début de l'implémentation des déplacements  ***/
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
        /***  fin de l'implémentation des déplacements  ***/

        /***  début de l'implémentation des attaques et créations de batiments pour les farmer  ***/

        //si l'unité peut attaquer, il attaque

        if(commande_canattack.execute(* unit_i, state, ennemy_objects)){

            int size_ennemy_list = ennemy_objects.size();
            std::uniform_int_distribution<int> dis_i(0,size_ennemy_list - 1);
            i_object = dis_i(randgen);

            //identification de la position de l'objet choisi aléatoirement
            state::Position position_ennemy = ennemy_objects[i_object]->getPosition();


            //terrain sur lequel l'object est situé
            state::Terrain * object_terrain = state.getMap().get()->getTerrain(position_ennemy.getX(),position_ennemy.getY()).get();
            commande_damage.execute(state,unit_i, ennemy_objects[i_object].get(),* object_terrain);
        }

        //si le farmeur n'a pas attaqué, il peut créer un batiment
        if(unit_i->getUnitType()==farmer){
            if(execute) {//une chance sur deux de créer un batiment (si il possède les ressources nécéssaire)
                int pos_x = unit_i->getPosition().getX();
                int pos_y = unit_i->getPosition().getY();
                for(shared_ptr<GameObject> obstacle: list_building){
                    if(obstacle.get()->getPosition() == unit_i->getPosition()){
                        cout<< "cannot build here, already a constructed building present!" <<endl;
                    }
                    else{
                        building_type = dis_buildings(randgen);
                        commande_create.execute(state, pos_x, pos_y, building_type, true);
                    }
                }

            }
        }
    }

    /***  implémentation des créations d'unités   ***/


    //randgen pour les différentes unités offensives: -1 signifie l'unité vide
    std::uniform_int_distribution<int> dis_units(infantry,maxUnit -1);
    int unit_type = -1;

    for(shared_ptr<Building> b : list_building){
        if(b.get()->getBuildingType()==state::town){
            if(execute){
               int pos_x = b.get()->getPosition().getX();
               int pos_y = b.get()->getPosition().getY();
               commande_create.execute(state,pos_x, pos_y, farmer, false);
            }
        }
        else if(b.get()->getBuildingType()==state::barrack){
            if(execute){
                int pos_x = b.get()->getPosition().getX();
                int pos_y = b.get()->getPosition().getY();
                unit_type = dis_units(randgen);
                commande_create.execute(state,pos_x, pos_y, unit_type, false);
            }
        }

    }





    //commande de fin de tour; préparation pour le joueur suivant
    commande_turn.execute(state);





    return true;
}



