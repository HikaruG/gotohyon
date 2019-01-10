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


    //récupération du joueur courant
    Player * current_player = state.getCurrentPlayer().get();

    //récupération de l'avantage actuel du joueur :
    //mise à jour de l'économie
    unsigned int food, gold;
    state.getCurrentPlayer().get()->getRessource(food, gold);
    //mise à jour de l'économie
    shared_ptr<engine::HandleGrowth> growth_check(new engine::HandleGrowth(food, gold));
    engine.addCommands(growth_check);
    unsigned int new_food, new_gold;
    current_player->getRessource(new_food, new_gold);
    cout << "player"<< current_player->getPlayerId() << "'s current gold is " << new_gold << endl;
    cout << "player"<< current_player->getPlayerId() << "'s current food is " << new_food << endl;


    //récupération de la liste des bâtiments du pc
    vector<shared_ptr<Building>> list_building = state.getCurrentPlayer()->getPlayerBuildingList();

    //récupération de la liste des unités du pc
    vector<shared_ptr<Unit>> list_unit = state.getCurrentPlayer()->getPlayerUnitList();


    //variables pour déterminer la nouvelle position de l'unité


    int old_x, old_y, new_x = 0, new_y = 0;

    //randgen de 1/2
    std::uniform_int_distribution<int> dis_half(0,1);
    int execute = dis_half(randgen);
    //randgen pour les différents batiments: -1 signifie le batiment vide
    std::uniform_int_distribution<int> dis_buildings(mine, state::maxBuilding - 1);
    int building_type = -1;


    for(int i= 0; i < (int)list_unit.size(); i++)
    {
        Unit * unit_i = list_unit[i].get();

        /***  début de l'implémentation des déplacements  ***/
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
        shared_ptr<engine::HandleMovement> movement_units (new engine::HandleMovement(new_x, new_y, unit_i));
        engine.addCommands(movement_units);
        /***  fin de l'implémentation des déplacements  ***/

        /***  début de l'implémentation des attaques et créations de batiments pour les farmer  ***/
        //si l'unité peut attaquer, il attaque
        state.resetInRange(); //s'assure de pas avoir une liste d'unités pré-remplie
        shared_ptr<engine::HandleCanAttack> canattack (new engine::HandleCanAttack(list_unit[i]));
        engine.addCommands(canattack);
        engine.execute(state);
        int size_ennemy_list = state.getInRange().size();
        if(size_ennemy_list != 0){
            int i_object;            //indice de l'ennemi choisi comme cible lors de l'attaque
            std::uniform_int_distribution<int> dis_i(0,size_ennemy_list - 1);
            i_object = dis_i(randgen);
            cout << "the chosen ennemy unit is : " << state.getInRange()[i_object]->getPlayerId()
                 << state.getInRange()[i_object]->getProperty()->getStringType() << endl;
            shared_ptr<engine::HandleDamage> damage_archer (new engine::HandleDamage(unit_i, state.getInRange()[i_object].get()));
            engine.addCommands(damage_archer);
            engine.execute(state);
        }

        //si le farmeur n'a pas attaqué, il peut créer un batiment
        if(unit_i->getUnitType()==farmer){
            if(execute) {//une chance sur deux de créer un batiment (si il possède les ressources nécéssaire)
                int pos_x = unit_i->getPosition().getX();
                int pos_y = unit_i->getPosition().getY();
                for(shared_ptr<GameObject> obstacle: list_building){
                    if(obstacle.get()->getPosition() == unit_i->getPosition()){
                        //cout<< "cannot build here, already a constructed building present!" <<endl;
                    }
                    else{
                        building_type = dis_buildings(randgen);
                        shared_ptr<engine::HandleCreation> create_building (new engine::HandleCreation(pos_x, pos_y,building_type,true));
                        engine.addCommands(create_building);
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
                shared_ptr<engine::HandleCreation> create_unit (new engine::HandleCreation(pos_x, pos_y,unit_type,false));
                engine.addCommands(create_unit);
            }
        }
        else if(b.get()->getBuildingType()==state::barrack){
            if(execute){
                int pos_x = b.get()->getPosition().getX();
                int pos_y = b.get()->getPosition().getY();
                unit_type = dis_units(randgen);
                shared_ptr<engine::HandleCreation> create_unit (new engine::HandleCreation(pos_x, pos_y,unit_type,false));
                engine.addCommands(create_unit);
            }
        }
    }

    //commande de fin de tour; préparation pour le joueur suivant
    shared_ptr<engine::HandleTurn> end_turn (new engine::HandleTurn());
    engine.addCommands(end_turn);

    return true;
}



