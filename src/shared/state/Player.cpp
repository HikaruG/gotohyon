//
// Created by hikaru on 07/11/18.
//

#include "Player.h"

using namespace state;

Player::Player(unsigned int player_id, unsigned int in_gold, unsigned int in_food,
               std::vector<state::GameObject *> player_object_list) {
    this->player_id = player_id;
    this->in_food = 600;
    this->in_gold = 600;
    this->player_object_list = player_object_list;
}

bool Player::getPlayerGOList(std::vector<state::GameObject *> &player_object_list) {
    player_object_list = this->player_object_list;
    return true;
}

bool Player::getPlayerId(unsigned int &player_id) {
    player_id = this->player_id;
    return true;
}

bool Player::getRessource(unsigned int &in_food, unsigned int &in_gold) {
    in_food = this->in_food;
    in_gold = this->in_gold;
    return true;
}

bool Player::addPlayerGameObject(GameObject * game_object) {
    this->player_object_list.push_back(game_object);
    return true;
}