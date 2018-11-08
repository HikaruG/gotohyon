//
// Created by ben on 23/10/18.
//
#include <iostream>
#include "State.h"


using namespace state;

bool State::initializeMap (Map new_map){
    this->game_map = new_map;
    return true;
}

Map* State::getMap (){
    return &game_map;
}

State::State(unsigned int player_number)
      :game_map() ,game_player(0,0,0,{0},{0})

{
    this->player_nbr = player_number;
    day_count = 0;

}

bool State::initializePlayer(state::Player new_player) {
    this->list_player.push_back(&new_player);
    this->game_player = new_player;
}


bool State::setCurrentPlayer(Player &player) {
    if(player_nbr < player.getPlayerId() ) {
        return false;
    }
    else {
        player = game_player;
    }
    return true;
}


Player * State::getCurrentPlayer ()
{
    return &game_player;
}

bool State::setDay(unsigned int day) {
    this->day_count = day;
}

bool State::getDayCount (unsigned int* day_count)
{
    *day_count = this->day_count;
    return true;
}


bool State::addGameObject(unsigned int player_id, state::Position position, bool is_static, int type) {
    GameObject * ptr_new_game_object;
    ptr_new_game_object = game_map.addGameObject(player_id, position, is_static, type);
    if(is_static) {
        game_player.addPlayerBuilding((Building *)ptr_new_game_object);
    }
    else{
        game_player.addPlayerUnit((Unit *)ptr_new_game_object);
    }
    return true;
}

