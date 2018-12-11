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
    cout << "player"<< current_player->getPlayerId() << "'s current gold is " << gold << endl;
    cout << "player"<< current_player->getPlayerId() << "'s current food is " << food << endl;


    //récupération de la liste des bâtiments du joueur courant
    vector<shared_ptr<Building>> my_list_building = current_player->getPlayerBuildingList();

    //récupération des données nécéssaires pour les différents types de batiments
    int count_barrack =0, count_turret = 0, count_mine =0, count_farm = 0;
    for(int i = 0; i < my_list_building.size(); i++){
        if(my_list_building[i].get()->getBuildingType() == turret)
            count_turret ++;
        if(my_list_building[i].get()->getBuildingType() == barrack)
            count_barrack ++;
        if(my_list_building[i].get()->getBuildingType() == mine)
            count_mine ++;
        if(my_list_building[i].get()->getBuildingType() == farm)
            count_farm ++;
    }

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

    int town_distance = 400;
    //récupération de la ville ennemie la plus proche
    for(Building * closest_town : towns){
        int ennemy_x = closest_town->getPosition().getX();
        int ennemy_y = closest_town->getPosition().getY();
        if(town_distance > distance_pos(ennemy_x, my_x, ennemy_y, my_y)) {
            town_playerid = closest_town->getPlayerId();
            town_x = ennemy_x;
            town_y = ennemy_y;
            town_distance = distance_pos(ennemy_x, my_x, ennemy_y, my_y);
        }
    }

    cout << "ennemy player id is : " << town_playerid << endl;
    Player * ennemy = state.getListPlayer()[town_playerid].get();


    engine::HandleMovement commande_movement = engine::HandleMovement();
    engine::HandleCanAttack commande_canattack = engine::HandleCanAttack();
    engine::HandleDamage commande_damage = engine::HandleDamage();
    engine::HandleTurn commande_turn = engine::HandleTurn();
    engine::HandleCreation commande_create = engine::HandleCreation();

    int building_type = -1;


    //variables pour déterminer la nouvelle position de l'unité
    int old_x, old_y, new_x = 0, new_y = 0, distance, movement_x, movement_y;


    /*** AI pour les unités ***/
    for(int i= 0; i < (int)my_list_unit.size(); i++) {

        Unit *unit_i = my_list_unit[i].get();
        //rajout : pour utiliser les nouvelles commandes
        state.setSelUnit(my_list_unit[i]);

        //liste de game_object ennemie
        vector<shared_ptr<GameObject>> ennemy_objects{}; //instancie au vecteur nul

        /*** cas d'un farmeur : il n'attaque pas, ne peut que construire***/

        if (unit_i->getUnitType() == farmer) {
            //ses positions
            int pos_x = unit_i->getPosition().getX();
            int pos_y = unit_i->getPosition().getY();

            //son déplacement est pour le moment aléatoire, l'important étant sa phase de construction
            int distance_x, distance_y;
            state::Position position_unit = unit_i->getPosition();
            old_y = (int)position_unit.getY();
            old_x = (int)position_unit.getX();
            distance_x = unit_i->getMovementRange();
            std::uniform_int_distribution<int> dis_x(-distance_x,distance_x);
            new_x = old_x + dis_x(randgen);
            distance_y = distance_x - abs(old_x - new_x);
            std::uniform_int_distribution<int> dis_y(-distance_y,distance_y);
            new_y = old_y + dis_y(randgen);
            position_unit.setPosition(new_x,new_y);

            //rajout : pour utiliser les nouvelles commandes
            state.setSelPosition(position_unit);
            commande_movement.execute(state);


            if(count_mine == 0)
                commande_create.execute(state, pos_x, pos_y, mine, true);
            if(count_farm == 0)
                commande_create.execute(state, pos_x, pos_y, farm, true);


            //randgen pour les différents batiments:
            std::uniform_int_distribution<int> dis_buildings(0, 9);
            int build_i = dis_buildings(randgen);

            //si déficite en nourriture
            if (food < 400) {
                if (build_i < 3)
                    building_type = farm;
                else if (build_i < 4)
                    building_type = mine;
                else
                    building_type = -1;
            }

            //si déficite en argent
            if (mine < 400) {
                if (build_i < 3)
                    building_type = mine;
                else if (build_i < 4)
                    building_type = farm;
                else
                    building_type = -1;
            }

            //si argent / food pas abondants
            if (food < 800 || gold < 800) {
                int build_i = dis_buildings(randgen);
                if (build_i == 0)
                    building_type = farm;
                else if (build_i == 1)
                    building_type = mine;
                else if (build_i == 2)
                    building_type = barrack;
                else if(build_i == 3)
                    building_type = turret;
                else
                    building_type = -1;
            }

            //si argent food abondants

            //si argent / food abondants
            if (food > 2000 || gold < 2000) {
                int build_i = dis_buildings(randgen);
                if (build_i == 0)
                    building_type = farm;
                else if (build_i == 1)
                    building_type = mine;
                else if (build_i < 4)
                    building_type = barrack;
                else if (build_i < 6)
                    building_type = turret;
                else
                    building_type = -1;
            }

            //si argent / food abondants +
            if (food > 4000 || gold < 4000) {
                int build_i = dis_buildings(randgen);
                if (build_i < 6)
                    building_type = barrack;
                else
                    building_type = turret;
            }

            if(count_barrack >= 2 && building_type == barrack)
                building_type = -1;
            if(count_turret >= 1 && building_type == turret)
                building_type = -1;

            for (shared_ptr<GameObject> obstacle: my_list_building) {
                if (obstacle.get()->getPosition() == unit_i->getPosition()) {
                    //cout<< "cannot build here, already a constructed building present!" <<endl;
                } else {
                    commande_create.execute(state, pos_x, pos_y, building_type, true);
                    current_player->getRessource(food, gold);
                    cout << "player" << current_player->getPlayerId() << "'s current gold is " << gold << endl;
                    cout << "player" << current_player->getPlayerId() << "'s current food is " << food << endl;
                }

            }
            unit_i->getProperty()->setAvailability(false); //rend inaccessible le villageois après la création du batiment
        }

        /*** cas d'un archer: il attaque à distance, sinon il se déplace et attaque***/

        if (unit_i->getUnitType() == archer) {
            if (commande_canattack.execute(*unit_i, state, ennemy_objects)) {

                shared_ptr<GameObject>ennemy_unit = ennemy_objects[0];
                int hp = ennemy_unit.get()->getProperty()->getHealth();

                for (shared_ptr<GameObject> weakest_ennemy : ennemy_objects) {
                    if (hp > weakest_ennemy.get()->getProperty()->getHealth()) {
                        ennemy_unit = weakest_ennemy;
                    }
                }
                cout << "the chosen ennemy unit is : " << ennemy_unit->getPlayerId()
                     << ennemy_unit->getProperty()->getStringType() << endl;

                //identification de la position de l'objet choisi aléatoirement
                state::Position position_ennemy = ennemy_unit->getPosition();

                //rajout : pour utiliser les nouvelles commandes
                state.setSelTarget(ennemy_unit);
                state.setSelPosition(position_ennemy);

                //execution de la commande damage
                commande_damage.execute(state);
            }
        }

        /*** cas général : Infantry || Archer qui n'a pas attaqué ***/
        if (unit_i->getProperty()->getAvailability()) {
            ennemy_objects = {}; //ré-initialise la liste des cibles disponibles


            /***  début de l'implémentation des déplacements  ***/
            state::Position position_unit = unit_i->getPosition();
            old_y = (int) position_unit.getY();
            old_x = (int) position_unit.getX();
            cout << " player" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType()
                 << "'s current position x : " << old_x << endl;
            cout << " player" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType()
                 << "'s current position y : " << old_y << endl;
            distance = unit_i->getMovementRange();
            //cout << "mouvements restants possible:  "<< distance << endl;



            //tant que l'unité peut se déplacer || tant qu'il n'a pas atteint la ville
            while (distance > 0) {
                /* les cas particuliers */

                //si l'objet se trouve à la bonne position en x et en y
                if (town_x == old_x && town_y == old_y - 1) {
                    distance = 0;
                    break;
                }
                if (town_x == old_x && town_y == old_y + 1) {
                    distance = 0;
                    break;
                }


                if (town_x == old_x - 1 && town_y == old_y) {
                    distance = 0;
                    break;
                }

                if (town_x + 1 == old_x && town_y == old_y) {
                    distance = 0;
                    break;
                }

                //si l'objet se trouve déjà à la bonne position en x
                if (town_x == old_x) {
                    movement_y = movement_finder(old_y, town_y, distance);
                    if (movement_y == 0) {
                        distance = 0;
                        break;
                    }
                    new_x = old_x;
                    new_y = old_y + movement_y -
                            1; //L'unité peut cheater => il pourra bouger d'une case en plus si l'unité atteint la ville
                    //cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s new position y : "<< new_y << endl;
                    distance -= abs(movement_y);
                    //cout << "mouvements restants possible:  "<< distance << endl;
                    break;
                }

                //si l'objet se trouve déjà à la bonne position en y
                if (town_y == old_y) {
                    movement_x = movement_finder(old_x, town_x, distance);
                    if (movement_x == 0) {
                        distance = 0;
                        break;
                    }
                    new_y = old_y;
                    new_x = old_x + movement_x -
                            1; //L'unité peut cheater => il pourra bouger d'une case en plus si l'unité atteint la ville
                    //cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s new position x : "<< new_x << endl;
                    distance -= abs(movement_x);
                    //cout << "mouvements restants possible:  "<< distance << endl;
                    break;
                }

                /* cas général */

                //si l'objet est autant éloigné en x qu'en y
                if (abs(town_x - old_x) == abs(town_y - old_y)) {
                    movement_x = movement_finder(old_x, town_x, distance);
                    if (movement_x == 0) {
                        distance = 0;
                        break;
                    }
                    new_y = old_y;
                    new_x = old_x + movement_x;
                    distance -= abs(movement_x);
                    //cout << "mouvements restants possible:  "<< distance << endl;
                    break;
                }

                //si l'objet est plus éloigné en x qu'en y
                if (abs(town_x - old_x) > abs(town_y - old_y)) {
                    movement_x = movement_finder(old_x, town_x, distance);
                    if (movement_x == 0) {
                        distance = 0;
                        break;
                    }
                    new_y = old_y;
                    new_x = old_x + movement_x;
                    distance -= abs(movement_x);
                    //cout << "mouvements restants possible:  "<< distance << endl;
                    break;
                }

                //si l'objet est plus éloigné en y qu'en x
                if (abs(town_x - old_x) < abs(town_y - old_y)) {
                    movement_y = movement_finder(old_y, town_y, distance);
                    if (movement_y == 0) {
                        distance = 0;
                        break;
                    }
                    new_x = old_x;
                    new_y = old_y + movement_y;
                    //cout <<" player no" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType() << "'s new position y : "<< new_y << endl;
                    distance -= abs(movement_y);
                    //cout << "mouvements restants possible:  "<< distance << endl;
                    break;
                }


            }


            cout << " player" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType()
                 << "'s new position x : " << new_x << endl;
            cout << " player" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType()
                 << "'s new position x : " << new_y << endl;

            position_unit.setPosition(new_x,new_y);
            //rajout : pour utiliser les nouvelles commandes
            state.setSelPosition(position_unit);

            commande_movement.execute(state);
            /***  fin de l'implémentation des déplacements  ***/

            /***  début de l'implémentation des attaques et créations de batiments pour les farmer  ***/

            //si l'unité peut attaquer, il attaque

            if (commande_canattack.execute(*unit_i, state, ennemy_objects)) {

                shared_ptr<GameObject>ennemy_unit = ennemy_objects[0];
                int hp = ennemy_unit.get()->getProperty()->getHealth();

                for (shared_ptr<GameObject> weakest_ennemy : ennemy_objects) {
                    if (hp > weakest_ennemy.get()->getProperty()->getHealth()) {
                        ennemy_unit = weakest_ennemy;
                    }
                }
                cout << "the chosen ennemy unit is : " << ennemy_unit->getPlayerId()
                     << ennemy_unit->getProperty()->getStringType() << endl;

                //identification de la position de l'objet le plus faible
                state::Position position_ennemy = ennemy_unit->getPosition();

                //rajout : pour utiliser les nouvelles commandes
                state.setSelPosition(position_ennemy);
                state.setSelTarget(ennemy_unit);

                //execution de la commande damage
                commande_damage.execute(state);
            }
        }
    }

    /***  implémentation des créations d'unités  ***/

    /*** AI pour les batiments ***/
    int ennemy_army_size = ennemy->getPlayerUnitList().size();
    int my_army_size = my_list_unit.size();

    int count_farmers = 0;
    for(int i = 0; i < my_list_unit.size(); i++){
        if(my_list_unit[i].get()->getUnitType() == farmer)
            count_farmers ++;
    }


    //randgen pour les différentes unités offensives: -1 signifie l'unité vide
    std::uniform_int_distribution<int> dis_farmers(1, 9);
    std::uniform_int_distribution<int> dis_units(infantry, maxUnit -1);
    int ai_farmers = dis_farmers(randgen);
    int unit_type = -1;

    for (shared_ptr<Building> b : my_list_building) {

        if (b.get()->getBuildingType() == state::town) {
            if(count_farmers < 3) {
                int pos_x = b.get()->getPosition().getX();
                int pos_y = b.get()->getPosition().getY();
                //si il est en manque de ressource, il aura plus de chances de créer des villageois

                if (food < 100 || gold < 100) {
                    if (ai_farmers < 8)
                        commande_create.execute(state, pos_x, pos_y, farmer, false);
                }
                //si il possède moyennement de l'argent, il a 2/9 de chances de créer un villageois
                if (food < 1500 || gold < 1500) {
                    if (ai_farmers < 2)
                        commande_create.execute(state, pos_x, pos_y, farmer, false);
                }
            }

        } else if (b.get()->getBuildingType() == state::barrack) {
            int pos_x = b.get()->getPosition().getX();
            int pos_y = b.get()->getPosition().getY();
            unit_type = dis_units(randgen);
            //si il est en manque de ressource, il aura quasiment pas de chances de créer une unité offensive
            if (food < 200 || gold < 200) {
                if (ai_farmers > 8)
                    commande_create.execute(state, pos_x, pos_y, unit_type, false);
            }
            //si il possède moyennement de l'argent, il a 1/3 de chances de créer un villageois
            if (food < 1500 || gold < 1500) {
                if (ai_farmers < 4)
                    commande_create.execute(state, pos_x, pos_y, unit_type, false);
            } else {
                if (ai_farmers > 2)
                    commande_create.execute(state, pos_x, pos_y, unit_type, false);
            }
        }
    }

    current_player->getRessource(food, gold);
    cout << "player"<< current_player->getPlayerId() << "'s current gold is " << gold << endl;
    cout << "player"<< current_player->getPlayerId() << "'s current food is " << food << endl;


    //commande de fin de tour; préparation pour le joueur suivant
    commande_turn.execute(state);
    return true;
}


