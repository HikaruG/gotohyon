//
// Created by hikaru on 13/11/18.
//

#include <state/Player.h>
#include <engine/Command.h>
#include "RandomAI.h"

using namespace ai;

RandomAI::RandomAI(int random_seed) {

}

bool RandomAI::run(engine::Engine &engine, state::Player& player, state::State& state) {

    //récuperation des listes de commandes pour l'IA
    std::vector<int>list_commands;
    listCommandType(list_commands,state,player);

    int moves_count = 0;
    //récupération de la liste des bâtiments du pc
    std::vector<state::Building *> list_building;
    player.getPlayerBuildingList(list_building);

    //récupération de la liste des unités du pc + nb d'unités
    std::vector<state::Unit *> * list_unit;
    player.getPlayerUnitList(* list_unit);



    for(int s : list_commands)
    {
        switch(s){
            case engine::HANDLE_MOVEMENT:

                break;



        }

    }


    return true;
}