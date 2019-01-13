//
// Created by hikaru on 07/11/18.
//

#include "Player.h"
#include <iostream>

using namespace state;
using namespace std;

Player::Player(size_t player_id, bool is_npc){
    this->player_id = player_id;
    this->is_npc = is_npc;
    this->in_food = 2000;
    this->in_gold = 2000;
    this->is_dead = false;
}


Player::Player() {
    this->player_id = 0;
    this->in_food = 600;
    this->in_gold = 600;
    this->is_dead = false;
}

Player::~Player() {
    cout << " player détruit " << this << endl;
}

bool Player::addPlayerUnit(shared_ptr<Unit> unit) {
    this->player_unit_list.push_back(unit);
    return true;
}

bool Player::addPlayerBuilding(shared_ptr<Building> building) {
    this->player_building_list.push_back(building);
    return true;
}


bool Player::deletePlayerBuilding(state::Building* deleting_building, bool keep_track) {
    size_t list_size =player_building_list.size();
    for(int i=0; i < (int)list_size; i++) {
        cout << " deleting building's type " << deleting_building->getBuildingType() << " with the id " << deleting_building->getGame_object_id() << endl;
        cout << " player building's type " << player_building_list[i].get()->getBuildingType() <<  " with the id " << player_building_list[i].get()->getGame_object_id() << endl;
        if(deleting_building->getGame_object_id()==player_building_list[i].get()->getGame_object_id()){
            //détruit le pointeur associé au batiment situé à la position i
            if(keep_track)
                    player_dead_building_list.push_back(player_building_list[i]);
            player_building_list.erase(player_building_list.begin() + i);
            break;
        }
    }
    if(list_size == player_building_list.size() + 1) {
        return true;
    }
    cout << "error deleting the building" << endl;
    cout << "list size = " << list_size << " and player building list size is " << player_building_list.size() << endl;
    return false;
}

bool Player::revivePlayerBuilding(unsigned int game_object_id) {
    for(int i = 0; i < player_dead_building_list.size(); i++){
        if(player_dead_building_list[i].get()->getGame_object_id() == game_object_id){
            player_building_list.push_back(player_dead_building_list[i]);
            player_dead_building_list.erase(player_dead_building_list.begin() + i);
            return true;
        }
    }
    return false;
}

bool Player::deletePlayerUnit(state::Unit* deleting_unit,bool keep_track) {
    size_t list_size = player_unit_list.size();
    for(int i=0; i<(int)player_unit_list.size(); i++) {
        if(deleting_unit->getGame_object_id()==player_unit_list[i].get()->getGame_object_id()){
            //détruit le pointeur associé à l'unité située à la position i
            if(keep_track)
                player_dead_unit_list.push_back(player_unit_list[i]);
            player_unit_list.erase(player_unit_list.begin() + i);
            break;
        }
    }
    if(list_size == player_unit_list.size() + 1) {
        return true;
    }
    cout << "error deleting the unit" << endl;
    return false;
}

bool Player::revivePlayerUnit(unsigned int game_object_id) {
    for(int i = 0; i < player_dead_unit_list.size(); i++){
        if(player_dead_unit_list[i].get()->getGame_object_id() == game_object_id){
            player_unit_list.push_back(player_dead_unit_list[i]);
            player_dead_unit_list.erase(player_dead_unit_list.begin() + i);
            return true;
        }
    }
    return false;
}

bool Player::checkAvailability(){
    for(shared_ptr<Building> b: this->player_building_list){
        if(b.get()->getProperty()->is_available){
            this->objects_available = true;
            return true;
        }
    }
    for(shared_ptr<Unit> u: this->player_unit_list){
        if(u.get()->getProperty()->is_available){
            this->objects_available = true;
            return true;
        }
    }
    this->objects_available = false;
    return false;
}

vector<shared_ptr<Building>>& Player::getPlayerBuildingList(){
    return player_building_list;
}

std::vector<std::shared_ptr<Unit>>& Player::getPlayerUnitList (){
    return player_unit_list;
}

bool Player::getAvailability(){
    return this->objects_available;
}

bool Player::getIsNpc(){
    return this->is_npc;
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

bool Player::setRessourceNet(unsigned int new_food_value, unsigned int new_gold_value) {
    this->in_food = new_food_value;
    this->in_gold = new_gold_value;
}

bool Player::getIsDead() {
    return this->is_dead;
}

bool Player::setIsDead() {
    this->is_dead = true;
    return true;
}

bool Player::setIsAlive() {
    this->is_dead = false;
    return true;
}

