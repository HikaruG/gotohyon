//
// Created by hikaru on 07/11/18.
//

#include "Player.h"

using namespace state;

Player::Player(unsigned int player_id, unsigned int in_gold, unsigned int in_food,
               std::vector<Building *>  building_list, std::vector<Unit *> unit_list) {
    this->player_id = player_id;
    this->in_food = 600;
    this->in_gold = 600;
    this->player_building_list = building_list;
    this->player_unit_list = unit_list;
}

bool Player::getPlayerUnitList(std::vector<state::Unit *> &unit_list) {
    unit_list = this->player_unit_list;
    return true;
}

bool Player::getPlayerBuildingList(std::vector<state::Building *> &building_list) {
    building_list = this->player_building_list;
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
}

bool Player::addPlayerUnit(state::Unit * unit) {
    this->player_unit_list.push_back(unit);
    return true;
}

bool Player::addPlayerBuilding(Building * building) {
    this->player_building_list.push_back(building);
    return true;
}
