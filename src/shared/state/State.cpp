//
// Created by ben on 23/10/18.
//
#include "State.h"


using namespace state;

bool State::initializeMap (Map new_map){
    this->game_map = new_map;
    return true;
}

bool State::getMap (Map * currentMap){
    *currentMap = this->game_map;
    return true;
}

State::State(unsigned int player_number)
        :game_map(1,1,{0})
{
    this->player_nbr = player_number;
    day_count = 0;
    current_player = 0;
}

bool State::getCurrentPlayer (unsigned int* current_player)
{
    *current_player = this->current_player;
    return true;
}

bool State::getDayCount (unsigned int* day_count)
{
    *day_count = this->day_count;
    return true;
}

bool State::getMyList(unsigned int player_id, std::vector<state::GameObject *> my_list) {
    std::vector<GameObject *> new_list;
    game_map.getListGameObject(new_list);
    for( state::GameObject * s: new_list){
        if (s->getPlayer_id() == player_id){
            my_list.push_back(s);
        }
    }
    return true;
}
