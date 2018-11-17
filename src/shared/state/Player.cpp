//
// Created by hikaru on 07/11/18.
//

#include "Player.h"

using namespace state;
using namespace std;

Player::Player(size_t player_id, size_t starting_gold, size_t starting_food){
    this->player_id = player_id;
    this->in_food = starting_food;
    this->in_gold = starting_gold;

}


Player::Player() {
    this->player_id = 0;
    this->in_food = 600;
    this->in_gold = 600;
}

Player::~Player() {

}

vector<shared_ptr<Building>>& Player::getPlayerBuildingList(){
    return player_building_list;
}



std::vector<std::shared_ptr<Unit>>& Player::getPlayerUnitList (){
    return player_unit_list;
}

unsigned int Player::getPlayerId() {
    return this->player_id;
}

bool Player::getRessource(unsigned int &in_food, unsigned int &in_gold) {
    in_food = this->in_food;
    in_gold = this->in_gold;
    return true;
}

bool Player::setRessource(unsigned int add_gold, unsigned int add_food) {
    in_gold += add_gold;
    in_food += add_food;
    return true;
}

bool Player::addPlayerUnit(shared_ptr<Unit> unit) {
    this->player_unit_list.push_back(unit);
    return true;
}

bool Player::addPlayerBuilding(shared_ptr<Building> building) {
    this->player_building_list.push_back(building);
    return true;
}
