#include <state/Player.h>
#include <engine/Command.h>
#include "DeepAI.h"
#include "engine.h"
#include <vector>
#include "iostream"

using namespace state;
using namespace std;
using namespace ai;
using namespace engine;

DeepAI::DeepAI(int random_seed){
    std::mt19937 randgen(random_seed);
    this->update_count = 0;
    this->attack_point = vector<int>(depth,0);
    this->creation_point = vector<int>(depth,0);
    this->movement_point = vector<int>(depth,0);
    this->total_point = -9999;
}

//notre deep AI = un bot ultra aggressif qui réduit le nombre d'unités au max:
//les conditions de gain: plus les unités ont attaqués, plus le point de cet état futur est élevé; on mémorisera l'état avec le plus haut score
int DeepAI::path_finder(state::State &state, state::Unit &unit) {
    int movement_range = unit.getMovementRange();
    std::vector<Unit*> list_units = {};
    for( shared_ptr<Player> players : state.getListPlayer()){
        if(players.get()->getPlayerId() != unit.getPlayerId()) {
            for(shared_ptr<Unit> units : players.get()->getPlayerUnitList()){
                list_units.push_back(units.get());
            }
        }
    }
}


int distance_position_DeepAI(int x1, int x2, int y1, int y2){
    int dist_x = abs(x1 - x2);
    int dist_y = abs(y1 - y2);
    return dist_x + dist_y;
}


bool collision_object_DeepAI(int pos_x, int pos_y, Unit* unit, State& state){
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



vector<int> disadvantage(State& state){
    int ennemy_archers, ennemy_infantries;
    int my_archers = 0, my_infantries = 0;
    int ennemy_id = 0;
    vector<int> number_advantage = {};
    for( shared_ptr<Player> players : state.getListPlayer()) {
        ennemy_archers = 0, ennemy_infantries = 0;
        if (players.get()->getPlayerId() != state.getCurrentPlayer().get()->getPlayerId()) {
            for(shared_ptr<Unit> units : players.get()->getPlayerUnitList()){
                if(units.get()->getUnitType() == archer)
                    ennemy_archers ++;
                else if(units.get()->getUnitType() == infantry)
                    ennemy_infantries ++;
            }
            if (state.getCurrentPlayer().get()->getPlayerUnitList().size() < players.get()->getPlayerUnitList().size())
                number_advantage[ennemy_id] --;
            else
                number_advantage[ennemy_id] ++;
        } else{
            for(shared_ptr<Unit> units : players.get()->getPlayerUnitList()){
                if(units.get()->getUnitType() == archer)
                    my_archers ++;
                else if(units.get()->getUnitType() == infantry)
                    my_infantries ++;
            }
        }
        if(my_archers < ennemy_archers && my_infantries < ennemy_infantries)
            number_advantage[ennemy_id] -= 2;
        else if(my_archers < ennemy_archers || my_infantries < ennemy_infantries)
            number_advantage[ennemy_id] --;
        else
            number_advantage[ennemy_id] += 2;
        ennemy_id ++;
    }

    for(int i : number_advantage){ // fait en sorte de seulement garder les valeurs négatives
        if(i > 0)
            i =0;
    }

    return number_advantage;
}


bool update_points(State& state){
}

bool DeepAI::run(engine::Engine &engine, state::State &state) {
    for(update_count; update_count < depth; update_count ++) {
        vector<shared_ptr<Command>> current_commands;

        //récupération du joueur courant
        Player *current_player = state.getCurrentPlayer().get();
        //récupération de l'avantage actuel du joueur :
        army_advantage = disadvantage(state);
        //récupération de l'économie actuelle du joueur:
        unsigned int food, gold;
        state.getCurrentPlayer().get()->getRessource(food, gold);
        //mise à jour de l'économie
        shared_ptr<engine::HandleGrowth> growth_check(new engine::HandleGrowth(food, gold));
        engine.addCommands(growth_check);
        current_commands.push_back(growth_check);

        unsigned int new_food, new_gold;
        current_player->getRessource(new_food, new_gold);
        cout << "player"<< current_player->getPlayerId() << "'s current gold is " << new_gold << endl;
        cout << "player"<< current_player->getPlayerId() << "'s current food is " << new_food << endl;

        /*** recherche du centre-ville ennemi ***/
        //récupération des villes ennemies
        Building *ennemy_town(new Building());
        Building *my_town(new Building());
        vector<Building *> towns;
        int my_x = -1, my_y = -1; //coord de la ville du joueur
        unsigned int town_playerid; //id du joueur possédant la ville que le joueur courant attaque

        for (shared_ptr<Player> players : state.getListPlayer()) {
            if (players.get()->getPlayerId() != current_player->getPlayerId()) {
                for (int i = 0; i < (int) players.get()->getPlayerBuildingList().size(); i++) {
                    ennemy_town = players.get()->getPlayerBuildingList()[i].get();
                    if (ennemy_town->getBuildingType() == town) {
                        towns.push_back(ennemy_town);
                        break;
                    }
                }
            } else { //liste de buildings du joueur courant
                for (int i = 0; i < (int) players.get()->getPlayerBuildingList().size(); i++) {
                    my_town = players.get()->getPlayerBuildingList()[i].get();
                    if (my_town->getBuildingType() == town) {
                        my_x = my_town->getPosition().getX();
                        my_y = my_town->getPosition().getY();
                        break;
                    }
                }
            }
        }
        int town_x, town_y;
        int town_distance = 4000;
        //récupération de la ville ennemie la plus proche
        for (Building *closest_town : towns) {
            int ennemy_x = closest_town->getPosition().getX();
            int ennemy_y = closest_town->getPosition().getY();
            if (town_distance > distance_position_DeepAI(ennemy_x, my_x, ennemy_y, my_y)) {
                town_playerid = closest_town->getPlayerId();
                town_x = ennemy_x;
                town_y = ennemy_y;
                town_distance = distance_position_DeepAI(town_x, my_x, town_y, my_y);
            }
        }
        if (town_distance == 4000)
            cout << "error, no ennemy town found !" << endl;
        cout << "ennemy player id is : " << town_playerid << endl;
        Player *ennemy = state.getListPlayer()[town_playerid].get();

        //récupération de la liste des bâtiments du joueur courant
        vector<shared_ptr<Building>> my_list_building = current_player->getPlayerBuildingList();
        //récupération des données nécéssaires pour les différents types de batiments
        int count_barrack = 0, count_turret = 0, count_mine = 0, count_farm = 0;
        for (int i = 0; i < (int) my_list_building.size(); i++) {
            if (my_list_building[i].get()->getBuildingType() == turret)
                count_turret++;
            if (my_list_building[i].get()->getBuildingType() == barrack)
                count_barrack++;
            if (my_list_building[i].get()->getBuildingType() == mine)
                count_mine++;
            if (my_list_building[i].get()->getBuildingType() == farm)
                count_farm++;
        }

        //récupération de la liste des unités du joueur courant
        vector<shared_ptr<Unit>> my_list_unit = current_player->getPlayerUnitList();
        //variables pour déterminer la nouvelle position de l'unité
        int old_x, old_y, new_x = 0, new_y = 0;

        /*** AI pour les unités ***/
        for (int i = 0; i < (int) my_list_unit.size(); i++) {
            Unit *unit_i = my_list_unit[i].get();

            /*** cas d'un farmeur : il n'attaque pas, ne peut que construire***/
            if (unit_i->getUnitType() == farmer) {
                //phase de déplacement:
                //son déplacement est pour le moment aléatoire, l'important étant sa phase de construction
                int distance_x, distance_y;
                state::Position position_unit = unit_i->getPosition();
                old_y = (int) position_unit.getY();
                old_x = (int) position_unit.getX();
                distance_x = unit_i->getMovementRange();
                std::uniform_int_distribution<int> dis_x(-distance_x, distance_x);
                new_x = old_x + dis_x(randgen);
                distance_y = distance_x - abs(old_x - new_x);
                std::uniform_int_distribution<int> dis_y(-distance_y, distance_y);
                new_y = old_y + dis_y(randgen);
                shared_ptr<engine::HandleMovement> movement_farmer(new engine::HandleMovement(new_x, new_y, unit_i));
                engine.addCommands(movement_farmer);
                current_commands.push_back(movement_farmer);


                //phase de construction:
                //construit une mine puis un moulin si il n'en possède aucun
                int building_type = -1;
                if (count_mine == 0)
                    building_type = mine;
                else if (count_farm == 0)
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
                        shared_ptr<engine::HandleCreation> create_building(
                                new engine::HandleCreation(new_x, new_y, building_type, true));
                        engine.addCommands(create_building);
                        current_commands.push_back(create_building);
                    }
                }
            }

            /*** cas d'un archer: il attaque à distance, sinon il se déplace et attaque***/
            if (unit_i->getUnitType() == archer) {
                //attaque à distance
                state.resetInRange(); //s'assure de pas avoir une liste d'unités pré-remplie
                shared_ptr<engine::HandleCanAttack> canattack_archers(new engine::HandleCanAttack(my_list_unit[i]));
                engine.addCommands(canattack_archers);
                current_commands.push_back(canattack_archers);
                engine.execute(state);
                if (state.getInRange().size() != 0) {
                    shared_ptr<GameObject> ennemy_unit = state.getInRange()[0];
                    int hp = ennemy_unit.get()->getProperty()->getHealth();
                    for (shared_ptr<GameObject> weakest_ennemy : state.getInRange()) {
                        if (hp > weakest_ennemy.get()->getProperty()->getHealth()) {
                            ennemy_unit = weakest_ennemy;
                        }
                    }
                    cout << "the chosen ennemy unit is : " << ennemy_unit->getPlayerId()
                         << ennemy_unit->getProperty()->getStringType() << endl;
                    shared_ptr<engine::HandleDamage> damage_archer(new engine::HandleDamage(unit_i, ennemy_unit.get()));
                    engine.addCommands(damage_archer);
                    current_commands.push_back(damage_archer);
                    engine.execute(state); //mutex en multi
                    attack_point[update_count]++;
                }
            }
            /*** cas général : Infantry || Archer qui n'a pas attaqué ***/
            if (unit_i->getProperty()->getAvailability()) {
                //phase de déplacement:
                int range = unit_i->getMovementRange();
                state::Position position_unit = unit_i->getPosition();
                old_y = (int) position_unit.getY();
                old_x = (int) position_unit.getX();
                cout << " player" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType()
                     << "'s current position x : " << old_x << endl;
                cout << " player" << unit_i->getPlayerId() << unit_i->getProperty()->getStringType()
                     << "'s current position y : " << old_y << endl;
                //tant que l'unité peut se déplacer il se dirige vers le centre-ville ennemi de manière aléatoire
                while (range > 0) {
                    //  début de l'implémentation des déplacements
                    int distance_x, distance_y;
                    state::Position position_unit = unit_i->getPosition();
                    old_y = (int) position_unit.getY();
                    old_x = (int) position_unit.getX();
                    distance_x = unit_i->getMovementRange();
                    std::uniform_int_distribution<int> dis_x(-distance_x, distance_x);
                    new_x = old_x + dis_x(randgen);
                    distance_y = distance_x - abs(old_x - new_x);
                    std::uniform_int_distribution<int> dis_y(-distance_y, distance_y);
                    new_y = old_y + dis_y(randgen);

                    //si il y a collision, l'objet le dévie (en x si le déplacement initial était en y, et en y si le mvt initial était en x)
                    if (collision_object_DeepAI(new_x, new_y, unit_i, state)) {
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
                    shared_ptr<engine::HandleMovement> movement_unit(new engine::HandleMovement(new_x, new_y, unit_i));
                    engine.addCommands(movement_unit);
                    current_commands.push_back(movement_unit);

                    //prise en compte des états futurs : partie Deep
                    if (distance_position_DeepAI(new_x, town_x, new_y, town_y) <
                        distance_position_DeepAI(old_x, town_x, old_y, town_y))
                        movement_point[update_count]++;
                    else
                        movement_point[update_count]--;
                }

                /***  début de l'implémentation des attaques pour les unités en général  ***/
                state.resetInRange();
                shared_ptr<engine::HandleCanAttack> canattack_unit(new engine::HandleCanAttack(my_list_unit[i]));
                engine.addCommands(canattack_unit);
                current_commands.push_back(canattack_unit);
                engine.execute(state);
                //prise en compte des états futurs : partie Deep
                if (state.getInRange().size() == 0) {
                    movement_point[update_count]--;
                    attack_point[update_count] -= 3;
                } else if (state.getInRange().size() == 1) {
                    movement_point[update_count]++;
                    attack_point[update_count] += 5;
                } else {
                    movement_point[update_count] += 3;
                    shared_ptr<GameObject> ennemy_unit = state.getInRange()[0];
                    int hp = ennemy_unit.get()->getProperty()->getHealth();
                    for (shared_ptr<GameObject> weakest_ennemy : state.getInRange()) {
                        if (hp > weakest_ennemy.get()->getProperty()->getHealth()) {
                            ennemy_unit = weakest_ennemy;
                        }
                    }
                    if (ennemy_unit.get()->getProperty()->isStatic()) {
                        movement_point[update_count] += 7;
                    }
                    cout << "the chosen ennemy unit is : " << ennemy_unit->getPlayerId()
                         << ennemy_unit->getProperty()->getStringType() << endl;
                    shared_ptr<engine::HandleDamage> damage_unit(new engine::HandleDamage(unit_i, ennemy_unit.get()));
                    engine.addCommands(damage_unit);
                    current_commands.push_back(damage_unit);
                }
            }
        }

        /***  implémentation des créations d'unités  ***/
        /*** AI pour les batiments ***/
        int ennemy_army_size = ennemy->getPlayerUnitList().size();
        int my_army_size = my_list_unit.size();
        int count_farmers = 0;
        for (int farmers = 0; farmers < (int) my_list_unit.size(); farmers++) {
            if (my_list_unit[farmers].get()->getUnitType() == farmer)
                count_farmers++;
        }
        //randgen pour les différentes unités offensives: -1 signifie l'unité vide
        std::uniform_int_distribution<int> dis_farmers(1, 9);
        std::uniform_int_distribution<int> dis_units(infantry, maxUnit - 1);
        int unit_type = -1;
        for (shared_ptr<Building> b : my_list_building) {
            int ai_farmers = dis_farmers(randgen);
            //centre-ville : création de villageois
            if (b.get()->getBuildingType() == state::town) {
                int pos_x = b.get()->getPosition().getX();
                int pos_y = b.get()->getPosition().getY();
                //si il est en manque de ressource, il aura plus de chances de créer des villageois
                if (count_farmers != 0)
                    ai_farmers /= count_farmers;
                if (food < 100 || gold < 100) {
                    if (ai_farmers > 4)
                        unit_type = farmer;
                }
                    //si il possède moyennement de l'argent, il a 2/9 de chances de créer un villageois
                else if (food < 1500 || gold < 1500) {
                    if (ai_farmers >= 8)
                        unit_type = farmer;
                }
                shared_ptr<engine::HandleCreation> create_unit(
                        new engine::HandleCreation(pos_x, pos_y, unit_type, false));
                engine.addCommands(create_unit);
                current_commands.push_back(create_unit);
            }
                //barrack : création d'unités offensives
            else if (b.get()->getBuildingType() == state::barrack) {
                int pos_x = b.get()->getPosition().getX();
                int pos_y = b.get()->getPosition().getY();
                unit_type = dis_units(randgen);
                shared_ptr<engine::HandleCreation> create_unit(
                        new engine::HandleCreation(pos_x, pos_y, unit_type, false));
                if (ennemy_army_size > my_army_size)
                    ai_farmers *= 0.75;
                //si il est en manque de ressource, il aura quasiment pas de chances de créer une unité offensive
                if (food < 200 || gold < 200) {
                    if (ai_farmers <= 2) {
                        engine.addCommands(create_unit);
                        current_commands.push_back(create_unit);
                    }

                }
                //si il possède moyennement de l'argent, il a 1/3 de chances de créer un villageois
                if (food < 1500 || gold < 1500) {
                    if (ai_farmers <= 4) {
                        engine.addCommands(create_unit);
                        current_commands.push_back(create_unit);
                    }

                } else {
                    if (ai_farmers <= 5) {
                        engine.addCommands(create_unit);
                        current_commands.push_back(create_unit);
                    }
                }
            }
        }

        vector<int> army_size = disadvantage(state);
        army_advantage[update_count] = accumulate(army_size.begin(), army_size.end(), 0);

        current_player->getRessource(food, gold);
        cout << "player" << current_player->getPlayerId() << "'s current gold is " << gold << endl;
        cout << "player" << current_player->getPlayerId() << "'s current food is " << food << endl;

        //récupération de la valeur finale du gain pour le deep, puis remet le state à l'état d'avant
        int accumulated_point = army_advantage[update_count] + attack_point[update_count]
                                + creation_point[update_count] + movement_point[update_count];


        if(accumulated_point > total_point){
            true_commands.clear();
            for(int j = 1; j <= current_commands.size(); j++){
                true_commands.push_back(current_commands[current_commands.size() - j]);
            }
        }
        engine.execute(state); //mutex
        engine.undo(state);
    }

    for(int i =0; true_commands.size(); i++){
        engine.addCommands(true_commands[i]);
    }


    //commande de fin de tour; préparation pour le joueur suivant
    shared_ptr<engine::HandleTurn> end_turn (new engine::HandleTurn());
    engine.addCommands(end_turn);
    return true;
}


DeepAI::~DeepAI() = default;
