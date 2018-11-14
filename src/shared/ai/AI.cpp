//
// Created by hikaru on 13/11/18.
//

#include <state/Player.h>
#include <state/Terrain.h>
#include <engine/CommandTypeId.h>
#include <state/BuildingType.h>
#include <state/GameObject.h>
#include "AI.h"
#include "engine.h"

using namespace ai;

AI::AI() {
    //par défaut, le joueur IA est le player avec le player_id = 0;
}

bool AI::listCommandType(std::vector<int> list, state::State &state,
                      state::Player &player) {


    //récupération du terrain
    state::Terrain * terrain;

    //récupération de la liste des bâtiments du pc
    std::vector<state::Building *> list_building;
    player.getPlayerBuildingList(list_building);

    //récupération de la liste des unités du pc + nb d'unités
    unsigned int unit_count;
    std::vector<state::Unit *> * list_unit;
    player.getPlayerUnitList(* list_unit);
    unit_count = list_unit->size();



    //récupération de la liste des joueurs + PC
    std::vector<state::Player *> list_player;
    //state.getListPlayer(list_player);


    //listes de joueurs et de ses gameobjects
    std::vector<state::Unit *> * ptr_ennemy_unit_list;
    std::vector<state::Building *> * ptr_ennemy_building_list;

    std::vector<state::GameObject *> * ennemy_unit_list;
    std::vector<state::Building *> * ennemy_building_list;

    for(state::Player *s : list_player){
        if(s->getPlayerId() != player.getPlayerId()){//fonctionne seulement pour 1 seul joueur contre 1 PC
            s->getPlayerUnitList(* ptr_ennemy_unit_list);
            s->getPlayerBuildingList(* ptr_ennemy_building_list);
        }
    }



    //les commandes pour les déplacements des unités
    engine::HandleAttack commande_attack = engine::HandleAttack();
    engine::HandleCreation commande_creation = engine::HandleCreation();
    int type=0;
    for(state::Unit * s : * list_unit ){
        type = s->getGame_object_id();
        switch(type){
            case state::infantry:
                s->getMovementRange();
                //commande_attack().execute()
        }


        list.push_back(engine::HANDLE_MOVEMENT);
        list.push_back(engine::HANDLE_DAMAGE);
    }
    //les commandes pour les créations d'unités: en fonction du nombre de casernes + villes
    for (state::Building * s : list_building){
       // if( s->getBuildingType() == state::BuildingType::barrack || s->getBuildingType() == state::BuildingType::town){
            list.push_back(engine::HANDLE_CREATION);
        //}
    }


    //les commandes pour les attaques des unités:
















    return true;
}