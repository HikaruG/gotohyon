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

int direction(int orig_pos, int dest_pos){
    int dist = dest_pos - orig_pos;
        if(dist == 0)
            return 0;
        else if(dist < 0)
            return -1;
        else
            return 1;
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

bool collision_object(int pos_x, int pos_y, Unit* unit, State& state){
    Position position = Position(pos_x,pos_y);
    //début gestion de la collision d'objets
    vector<shared_ptr<state::Player>> list_player = state.getListPlayer();
    for (int i = 0; i < (int)list_player.size(); i++){

        //si le joueur dans la liste list_player n'est pas le joueur actuel
        if(list_player[i].get()->getPlayerId() != state.getCurrentPlayerId()){
            //récupère les listes des batiments et des unités des ennemis
            vector<shared_ptr<state::Unit>>ennemy_units = list_player[i].get()->getPlayerUnitList();
            vector<shared_ptr<state::Building>> ennemy_buildings = list_player[i].get()->getPlayerBuildingList();

            //vérifie si il y a pas une unité ennemie déja présente à cette position
            for(int i = 0; i < (int)ennemy_buildings.size(); i++){
                // si une unité est déjà présente, le déplacement devient impossible
                if(ennemy_buildings[i].get()->getPosition() == position){
                    return true;
                }
            };
            //vérifie si il n'y a pas un batiment ennemi déjà présent à cette position
            for(int i = 0; i < (int)ennemy_units.size(); i++){
                //si un batiment est déjà présent, le déplacement devient impossible
                if(ennemy_units[i].get()->getPosition() == position){
                    return true;
                }
            };
        }
        //si le joueur dans la liste list_player est le joueur actuel
        else{
            vector<shared_ptr<state::Unit>>ally_units = list_player[i].get()->getPlayerUnitList();
            for(int i =0; i < (int)ally_units.size(); i++){
                if(ally_units[i].get()->getPosition() == position){
                    return true;
                }
            }
        }
    }
    //fin gestion de la collision d'objets
    return false;
}


HeuristicAI::HeuristicAI(int random_seed) {
    std::mt19937 randgen(random_seed);
}

HeuristicAI::~HeuristicAI() = default;

bool HeuristicAI::run(engine::Engine &engine, state::State &state) {

    //récupération du joueur courant
    Player * current_player = state.getCurrentPlayer().get();

    //récupération de l'avantage actuel du joueur :
    unsigned int food, gold;
    state.getCurrentPlayer().get()->getRessource(food, gold);
    //mise à jour de l'économie
    shared_ptr<engine::HandleGrowth> growth_check(new engine::HandleGrowth(food, gold));
    engine.addCommands(growth_check);
    unsigned int new_food, new_gold;
    current_player->getRessource(new_food, new_gold);
    cout << "player"<< current_player->getPlayerId() << "'s current gold is " << new_gold << endl;
    cout << "player"<< current_player->getPlayerId() << "'s current food is " << new_food << endl;


    //récupération de la liste des bâtiments du joueur courant
    vector<shared_ptr<Building>> my_list_building = current_player->getPlayerBuildingList();

    //récupération des données nécéssaires pour les différents types de batiments
    int count_barrack =0, count_turret = 0, count_mine =0, count_farm = 0;
    for(int i = 0; i < (int)my_list_building.size(); i++){
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

    int town_distance = 4000;
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
    if(town_distance == 4000)
        cout << "error, no ennemy town found !" << endl;

    cout << "ennemy player id is : " << town_playerid << endl;
    Player * ennemy = state.getListPlayer()[town_playerid].get();


    //variables pour déterminer la nouvelle position de l'unité
    int old_x, old_y, new_x = 0, new_y = 0, distance, movement_x, movement_y;


    /*** AI pour les unités ***/
    for(int i= 0; i < (int)my_list_unit.size(); i++) {

        Unit *unit_i = my_list_unit[i].get();

        /*** cas d'un farmeur : il n'attaque pas, ne peut que construire***/
        if (unit_i->getUnitType() == farmer) {
            //phase de déplacement:
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
            shared_ptr<engine::HandleMovement> movement_farmer (new engine::HandleMovement(new_x, new_y, unit_i));
            engine.addCommands(movement_farmer);

            //phase de construction:
            //construit une mine puis un moulin si il n'en possède aucun
            int building_type = -1;
            if(count_mine == 0)
                building_type = mine;
            else if(count_farm == 0)
                building_type = farm;
            //sinon, construit en fonction de son économie
            else {
                std::uniform_int_distribution<int> dis_buildings(0, 9);
                int build_i = dis_buildings(randgen);
                //si déficite en nourriture
                if (food < 400) {
                    building_type = farm;
                }
                //si déficite en argent
                if (gold < 400) {
                    building_type = mine;
                }
                //si argent / food pas abondants
                if (food < 800 || gold < 800) {
                    if (build_i == 0)
                        building_type = farm;
                    else if (build_i == 1)
                        building_type = mine;
                    else if (build_i == 2)
                        building_type = barrack;
                    else if (build_i == 3)
                        building_type = turret;
                    else
                        building_type = -1;
                }
                //si argent / food abondants
                if (food > 2000 || gold > 2000) {
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
                    if (build_i < 6)
                        building_type = barrack;
                    else
                        building_type = turret;
                }
                //si le nombre max par type a été construit on ne construit rien
                if (count_barrack >= 2 && building_type == barrack)
                    building_type = -1;
                if (count_turret >= 1 && building_type == turret)
                    building_type = -1;
                if (count_mine >= 1 && building_type == mine)
                    building_type = -1;
                if (count_farm >= 1 && building_type == farm)
                    building_type = -1;
            }
            for (shared_ptr<GameObject> obstacle: my_list_building) {
                if (obstacle.get()->getPosition() == unit_i->getPosition()) {
                    //cout<< "cannot build here, already a constructed building present!" <<endl;
                } else {
                    shared_ptr<engine::HandleCreation> create_building (new engine::HandleCreation(new_x, new_y,building_type,true));
                    engine.addCommands(create_building);
                }
            }
        }


        /*** cas d'un archer: il attaque à distance, sinon il se déplace et attaque***/
        if (unit_i->getUnitType() == archer) {
            state.resetInRange(); //s'assure de pas avoir une liste d'unités pré-remplie
            shared_ptr<engine::HandleCanAttack> canattack_archers (new engine::HandleCanAttack(my_list_unit[i]));
            engine.addCommands(canattack_archers);
            engine.execute(state);

            if (state.getInRange().size() != 0) {
                shared_ptr<GameObject>ennemy_unit = state.getInRange()[0];
                int hp = ennemy_unit.get()->getProperty()->getHealth();
                for (shared_ptr<GameObject> weakest_ennemy : state.getInRange()) {
                    if (hp > weakest_ennemy.get()->getProperty()->getHealth()) {
                        ennemy_unit = weakest_ennemy;
                    }
                }
                cout << "the chosen ennemy unit is : " << ennemy_unit->getPlayerId()
                     << ennemy_unit->getProperty()->getStringType() << endl;
                shared_ptr<engine::HandleDamage> damage_archer (new engine::HandleDamage(unit_i, ennemy_unit.get()));
                engine.addCommands(damage_archer);
                engine.execute(state);
            }
        }


        /*** cas général : Infantry || Archer qui n'a pas attaqué ***/
        if (unit_i->getProperty()->getAvailability()) {
            //phase de déplacement:
            state::Position position_unit = unit_i->getPosition();
            old_y = (int) position_unit.getY();
            old_x = (int) position_unit.getX();
            cout << " player" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType()
                 << "'s current position x : " << old_x << endl;
            cout << " player" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType()
                 << "'s current position y : " << old_y << endl;
            distance = unit_i->getMovementRange();
            //tant que l'unité peut se déplacer il se dirige vers le centre-ville ennemi
            while (distance > 0) {
                /* les cas particuliers */
                //si l'objet se trouve à la bonne position en x et en y
                if (town_x == old_x && town_y == old_y - 1) {
                    break;
                } else if (town_x == old_x && town_y == old_y + 1) {
                    break;
                } else if (town_x == old_x - 1 && town_y == old_y) {
                    break;
                } else if (town_x + 1 == old_x && town_y == old_y) {
                    break;
                }
                    //si l'objet se trouve déjà en pos.x ou pos.y bon
                else {
                    //si l'objet se trouve déjà à la bonne position en x
                    if (town_x == old_x) {
                        movement_y = direction(old_y, town_y);
                        if (movement_y == 0) {
                            break;
                        }
                        new_x = old_x;
                        new_y = old_y + movement_y - 1;
                        distance -= abs(movement_y);
                    }
                        //si l'objet se trouve déjà à la bonne position en y
                    else if (town_y == old_y) {
                        movement_x = direction(old_x, town_x);
                        if (movement_x == 0) {
                            break;
                        }
                        new_y = old_y;
                        new_x = old_x + movement_x - 1;
                        distance -= abs(movement_x);
                    }
                        //si ni l'une ni l'autre position est la bonne
                    else {
                        //si l'objet est autant éloigné en x qu'en y
                        if (abs(town_x - old_x) == abs(town_y - old_y)) {
                            movement_x = direction(old_x, town_x);
                            new_y = old_y;
                            new_x = old_x + movement_x;
                            distance -= abs(movement_x);
                        }
                            //si l'objet est plus éloigné en x qu'en y
                        else if (abs(town_x - old_x) > abs(town_y - old_y)) {
                            movement_x = direction(old_x, town_x);
                            new_y = old_y;
                            new_x = old_x + movement_x;
                            distance -= abs(movement_x);
                        }
                            //si l'objet est plus éloigné en y qu'en x
                        else if (abs(town_x - old_x) < abs(town_y - old_y)) {
                            movement_y = direction(old_y, town_y);
                            new_x = old_x;
                            new_y = old_y + movement_y;
                            distance -= abs(movement_y);
                        }
                    }
                    //si il y a collision, l'objet le dévie (en x si le déplacement initial était en y, et en y si le mvt initial était en x)
                    if(collision_object(new_x,new_y,unit_i,state)) {
                        std::uniform_int_distribution<int> dis_side(0, 9);
                        int dogde = dis_side(randgen);
                        if (dogde < 5) {
                            if (new_x == old_x) { //le déplacement initial était celui en y:
                                new_x -= 1;
                                new_y = old_y; //le déplacement problématique est annulé
                            } else { //le déplacement intial était celui en x:
                                new_y -= 1;
                                new_x = old_x; //le déplacement problématique est annulé
                            }
                        } else {
                            if (new_x == old_x) { //le déplacement initial était celui en y:
                                new_x += 1;
                                new_y = old_y; //le déplacement problématique est annulé
                            } else { //le déplacement intial était celui en x:
                                new_y += 1;
                                new_x = old_x; //le déplacement problématique est annulé
                            }
                        }
                    }

                }

                shared_ptr<engine::HandleMovement> movement_unit (new engine::HandleMovement(new_x, new_y, unit_i));
                engine.addCommands(movement_unit);
            }

            /***  début de l'implémentation des attaques pour les unités en général  ***/
            state.resetInRange();
            shared_ptr<engine::HandleCanAttack> canattack_unit (new engine::HandleCanAttack(my_list_unit[i]));
            engine.addCommands(canattack_unit);
            engine.execute(state);

            if (state.getInRange().size() != 0) {
                shared_ptr<GameObject>ennemy_unit = state.getInRange()[0];
                int hp = ennemy_unit.get()->getProperty()->getHealth();
                for (shared_ptr<GameObject> weakest_ennemy : state.getInRange()) {
                    if (hp > weakest_ennemy.get()->getProperty()->getHealth()) {
                        ennemy_unit = weakest_ennemy;
                    }
                }
                cout << "the chosen ennemy unit is : " << ennemy_unit->getPlayerId()
                     << ennemy_unit->getProperty()->getStringType() << endl;
                shared_ptr<engine::HandleDamage> damage_unit (new engine::HandleDamage(unit_i, ennemy_unit.get()));
                engine.addCommands(damage_unit);
            }
        }
    }

    /***  implémentation des créations d'unités  ***/
    /*** AI pour les batiments ***/
    int ennemy_army_size = ennemy->getPlayerUnitList().size();
    int my_army_size = my_list_unit.size();
    int count_farmers = 0;
    for(int i = 0; i < (int)my_list_unit.size(); i++){
        if(my_list_unit[i].get()->getUnitType() == farmer)
            count_farmers ++;
    }
    //randgen pour les différentes unités offensives: -1 signifie l'unité vide
    std::uniform_int_distribution<int> dis_farmers(1, 9);
    std::uniform_int_distribution<int> dis_units(infantry, maxUnit -1);
    int unit_type = -1;
    for (shared_ptr<Building> b : my_list_building) {
        int ai_farmers = dis_farmers(randgen);
        //centre-ville : création de villageois
        if (b.get()->getBuildingType() == state::town) {
            int pos_x = b.get()->getPosition().getX();
            int pos_y = b.get()->getPosition().getY();
            //si il est en manque de ressource, il aura plus de chances de créer des villageois
            if(count_farmers != 0)
                ai_farmers/=count_farmers;
            if (food < 100 || gold < 100) {
                if (ai_farmers > 4)
                    unit_type = farmer;
            }
            //si il possède moyennement de l'argent, il a 2/9 de chances de créer un villageois
            else if (food < 1500 || gold < 1500) {
                if (ai_farmers >= 8)
                    unit_type = farmer;
            }


            shared_ptr<engine::HandleCreation> create_unit (new engine::HandleCreation(pos_x, pos_y,unit_type,false));
            engine.addCommands(create_unit);
        }
        //barrack : création d'unités offensives
        else if (b.get()->getBuildingType() == state::barrack) {
            int pos_x = b.get()->getPosition().getX();
            int pos_y = b.get()->getPosition().getY();
            unit_type = dis_units(randgen);
            shared_ptr<engine::HandleCreation> create_unit (new engine::HandleCreation(pos_x, pos_y,unit_type,false));
            if(ennemy_army_size > my_army_size)
                ai_farmers*=0.75;
            //si il est en manque de ressource, il aura quasiment pas de chances de créer une unité offensive
            if (food < 200 || gold < 200) {
                if (ai_farmers <= 2)
                    engine.addCommands(create_unit);
            }
            //si il possède moyennement de l'argent, il a 1/3 de chances de créer un villageois
            if (food < 1500 || gold < 1500) {
                if (ai_farmers <= 4)
                    engine.addCommands(create_unit);
            } else {
                if (ai_farmers <= 5)
                    engine.addCommands(create_unit);
            }
        }
    }
    current_player->getRessource(food, gold);
    cout << "player"<< current_player->getPlayerId() << "'s current gold is " << gold << endl;
    cout << "player"<< current_player->getPlayerId() << "'s current food is " << food << endl;
    //commande de fin de tour; préparation pour le joueur suivant

    shared_ptr<engine::HandleTurn> end_turn (new engine::HandleTurn());
    engine.addCommands(end_turn);

    return true;
}

