//
// Created by ben on 05/12/18.
//
#include <state/Player.h>
#include <engine/Command.h>
#include "HeuristicAI.h"
#include "engine.h"
#include <iostream>


using namespace ai;
using namespace state;
using namespace std;

int movement_finder(int orig_pos, int dest_pos, int mvt_range){
    int dist = dest_pos - orig_pos;
    cout << "distance " << dist << endl;
        if(abs(dist) > mvt_range) {
            if(dist < 0)
                return -mvt_range;
            else
                return mvt_range;
        }
        else {
            return dist;
        }
}

int distance_pos(int x1, int x2, int y1, int y2){
    int dist_x = abs(x1 - x2);
    int dist_y = abs(y1 - y2);
    return dist_x + dist_y;
}

bool number_advantage(int my_list, int ennemy_list){
    if(my_list > ennemy_list)
        return true;
    return false;
}

bool ressource_check(int food, int gold){
    if(food > 1000)
        if(gold > 1000)
            return true;
    return false;
}

HeuristicAI::HeuristicAI(int random_seed) {
    std::mt19937 randgen(random_seed);
}

HeuristicAI::~HeuristicAI() = default;

bool HeuristicAI::run(engine::Engine &engine, state::State &state) {

    //récupération du joueur courant
    Player * current_player = state.getCurrentPlayer().get();

    //mise à jour de l'économie
    engine::HandleGrowth commande_growth = engine::HandleGrowth();
    commande_growth.execute(state);
    unsigned int food, gold;
    current_player->getRessource(food, gold);
    cout << "current gold is " << gold << endl;
    cout << "current food is " << food << endl;


    //récupération de la liste des bâtiments du joueur courant
    vector<shared_ptr<Building>> my_list_building = current_player->getPlayerBuildingList();

    //récupération de la liste des unités du joueur courant
    vector<shared_ptr<Unit>> my_list_unit = current_player->getPlayerUnitList();




    /*** recherche du centre-ville ennemi ***/

    //récupération des villes ennemies
    Building * ennemy_town (new Building());
    Building * my_town (new Building());
    vector<Building *> towns;
    int town_x = -1, town_y = -1, my_x = -1, my_y = -1; //positions des towns
    unsigned int town_playerid; //id du joueur possédant la ville que le joueur courant attaque


    for (shared_ptr<Player> players : state.getListPlayer()) {
        if (players.get()->getPlayerId() != current_player->getPlayerId()) {
            for (int i = 0; i < (int)players.get()->getPlayerBuildingList().size(); i++) {
                ennemy_town = players.get()->getPlayerBuildingList()[i].get();
                if (ennemy_town->getBuildingType() == town) {
                    towns.push_back(ennemy_town);
                    break;
                }
            }
        } else { //liste de buildings du joueur courant
            for (int i = 0; i < (int)players.get()->getPlayerBuildingList().size(); i++) {
                my_town = players.get()->getPlayerBuildingList()[i].get();
                if (my_town->getBuildingType() == town) {
                    my_x = my_town->getPosition().getX();
                    my_y = my_town->getPosition().getY();
                    break;
                }
            }
        }
    }

    int town_distance = 40;
    //récupération de la ville ennemie la plus proche
    for(Building * closest_town : towns){
        int ennemy_x = closest_town->getPosition().getX();
        int ennemy_y = closest_town->getPosition().getY();
        if(town_distance > distance_pos(ennemy_x, my_x, ennemy_y, my_y))
            town_playerid = closest_town->getPlayerId();
            town_x = ennemy_x;
            town_y = ennemy_y;
            town_distance = distance_pos(ennemy_x, my_x, ennemy_y, my_y);
    }


    engine::HandleMovement commande_movement = engine::HandleMovement();
    engine::HandleCanAttack commande_canattack = engine::HandleCanAttack();
    engine::HandleDamage commande_damage = engine::HandleDamage();
    engine::HandleTurn commande_turn = engine::HandleTurn();
    engine::HandleCreation commande_create = engine::HandleCreation();

    //liste de game_object ennemie
    vector<shared_ptr<GameObject>> ennemy_objects;



    int building_type = -1;


    //variables pour déterminer la nouvelle position de l'unité
    int old_x, old_y, new_x = 0, new_y = 0, distance, movement_x, movement_y;


    /*** AI pour les unités ***/
    for(int i= 0; i < (int)my_list_unit.size(); i++)
    {
        Unit * unit_i = my_list_unit[i].get();

        /***  début de l'implémentation des déplacements  ***/
        state::Position position_unit = unit_i->getPosition();
        old_y = (int)position_unit.getY();
        old_x = (int)position_unit.getX();
        cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s current position x : "<< old_x << endl;
        cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s current position y : "<< old_y << endl;
        distance = unit_i->getMovementRange();
        //cout << "mouvements restants possible:  "<< distance << endl;



        //tant que l'unité peut se déplacer || tant qu'il n'a pas atteint la ville
        while (distance > 0) {
            //les cas particuliers

            //si l'objet se trouve à la bonne position en x et en y
            if(town_x == old_x && town_y == old_y -1) {
                distance = 0;
                break;
            }
            if(town_x == old_x && town_y == old_y + 1){
                distance = 0;
                break;
            }


            if(town_x == old_x - 1 && town_y == old_y){
                distance = 0;
                break;
            }

            if(town_x + 1  == old_x && town_y == old_y){
                distance = 0;
                break;
            }

            //si l'objet se trouve déjà à la bonne position en x
            if (town_x == old_x) {
                movement_y = movement_finder(old_y, town_y,distance);
                if(movement_y == 0){
                    distance = 0;
                    break;
                }
                new_x = old_x;
                new_y = old_y + movement_y - 1; //L'unité peut cheater => il pourra bouger d'une case en plus si l'unité atteint la ville
                //cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s new position y : "<< new_y << endl;
                distance -= abs(movement_y);
                cout << "mouvements restants possible:  "<< distance << endl;
                break;
            }

            //si l'objet se trouve déjà à la bonne position en y
            if (town_y == old_y) {
                movement_x = movement_finder(old_x, town_x, distance);
                if(movement_x == 0){
                    distance = 0;
                    break;
                }
                new_y = old_y;
                new_x = old_x + movement_x - 1; //L'unité peut cheater => il pourra bouger d'une case en plus si l'unité atteint la ville
                //cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s new position x : "<< new_x << endl;
                distance -= abs(movement_x);
                cout << "mouvements restants possible:  "<< distance << endl;
                break;
            }

            //cas général

            //si l'objet est plus éloigné en x qu'en y
            if (abs(town_x - old_x) > abs(town_y - old_y)) {
                movement_x = movement_finder(old_x, town_x, distance);
                if(movement_x == 0){
                    distance = 0;
                    break;
                }
                new_y = old_y;
                new_x = old_x + movement_x;
                distance -= abs(movement_x);
                cout << "mouvements restants possible:  "<< distance << endl;
                break;
            }

            //si l'objet est plus éloigné en y qu'en x
            if (abs(town_x - old_x) < abs(town_y - old_y)) {
                movement_y = movement_finder(old_y, town_y, distance);
                if(movement_y == 0){
                    distance = 0;
                    break;
                }
                cout << "mouvement en y de "<< movement_y << endl;
                new_x = old_x;
                new_y = old_y + movement_y;
                //cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s new position y : "<< new_y << endl;
                distance -= abs(movement_y);
                cout << "mouvements restants possible:  "<< distance << endl;
                break;
            }
        }

        cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s new position x : "<< new_x << endl;
        cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s new position x : "<< new_y << endl;
        commande_movement.execute(*unit_i, state, new_x, new_y);
        /***  fin de l'implémentation des déplacements  ***/


        /***  début de l'implémentation des attaques et créations de batiments pour les farmer  ***/

        //si l'unité peut attaquer, il attaque

        if(commande_canattack.execute(* unit_i, state, ennemy_objects)){

            GameObject * ennemy_unit = ennemy_objects[0].get();
            int hp = ennemy_unit->getProperty()->getHealth();

            for(shared_ptr<GameObject> weakest_ennemy : ennemy_objects){
                if(hp > weakest_ennemy.get()->getProperty()->getHealth()){
                    ennemy_unit = weakest_ennemy.get();
                }
            }

            //identification de la position de l'objet choisi aléatoirement
            state::Position position_ennemy = ennemy_unit->getPosition();


            //terrain sur lequel l'object est situé
            state::Terrain * object_terrain = state.getMap().get()->getTerrain(position_ennemy.getX(),position_ennemy.getY()).get();

            //execution de la commande damage
            commande_damage.execute(state,unit_i, ennemy_unit,* object_terrain);
        }

        //si le farmeur n'a pas attaqué, il peut créer un batiment
        if(unit_i->getUnitType()==farmer){
            //créer un batiment seulement si ressource > 1000
            //en priorité les farms et les mines
            if(ressource_check(food, gold)) {
                //randgen pour les différents batiments:
                std::uniform_int_distribution<int> dis_buildings(0, 9);
                int build_i = dis_buildings(randgen);
                if (build_i == 0 || build_i == 1 || build_i == 2)
                    building_type = mine;
                if (build_i == 3 || build_i == 4 || build_i == 5)
                    building_type = farm;
                if (build_i == 6 || build_i == 7)
                    building_type = barrack;
                if (build_i == 8 || build_i == 9)
                    building_type = turret;

                int pos_x = unit_i->getPosition().getX();
                int pos_y = unit_i->getPosition().getY();
                for (shared_ptr<GameObject> obstacle: my_list_building) {
                    if (obstacle.get()->getPosition() == unit_i->getPosition()) {
                        //cout<< "cannot build here, already a constructed building present!" <<endl;
                    } else {
                        commande_create.execute(state, pos_x, pos_y, building_type, true);
                        current_player->getRessource(food, gold);
                        cout << "current gold is " << gold << endl;
                        cout << "current food is " << food << endl;
                    }

                }
            }

            }
        }

    /***  implémentation des créations d'unités  c ***/

    /*** AI pour les batiments ***/

    if(number_advantage(my_list_unit.size(), state.getListPlayer()[town_playerid].get()->getPlayerUnitList().size())) {

        //randgen pour les différentes unités offensives: -1 signifie l'unité vide
        std::uniform_int_distribution<int> dis_units(infantry, maxUnit - 1);
        int unit_type = -1;

        for (shared_ptr<Building> b : my_list_building) {
            if (b.get()->getBuildingType() == state::town) {
                    int pos_x = b.get()->getPosition().getX();
                    int pos_y = b.get()->getPosition().getY();
                    commande_create.execute(state, pos_x, pos_y, farmer, false);
            }
            else if (b.get()->getBuildingType() == state::barrack) {
                    int pos_x = b.get()->getPosition().getX();
                    int pos_y = b.get()->getPosition().getY();
                    unit_type = dis_units(randgen);
                    commande_create.execute(state, pos_x, pos_y, unit_type, false);
            }
        current_player->getRessource(food, gold);
        cout << "current gold is " << gold << endl;
        cout << "current food is " << food << endl;
        }
        }

    //commande de fin de tour; préparation pour le joueur suivant
    commande_turn.execute(state);
    return true;
}

