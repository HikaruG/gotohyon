//
// Created by ben on 15/10/18.
//
#include "Property.h"

using namespace state;

Property::Property(std::string type_name, unsigned int defense, unsigned int attack, unsigned int health_max) {
    this->type_game_object = type_name;
    this->attack = attack;
    this->defense = defense;
    this->health_max = health_max;
}

Property::Property() {
    //default constructor
    this->type_game_object = "default";
    this->attack = 0;
    this->defense = 0;
    this->health_max = 10;
}

Property& Property::operator=(const state::Property& orig) {
    this->type_game_object = orig.type_game_object;
    this->attack = orig.attack;
    this->defense = orig.defense;
    this->health_max = orig.health_max;
    return *this;
}

unsigned int Property::get_attack() {
    return this->attack;
}

unsigned int Property::get_defense() {
    return this->defense;
}

unsigned int Property::get_health_max() {
    return this->health_max;
}