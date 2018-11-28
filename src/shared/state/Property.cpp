//
// Created by ben on 15/10/18.
//
#include "Property.h"

using namespace state;

Property::Property(std::string type_name, unsigned int defense, unsigned int attack, unsigned int health_max,bool is_static,bool is_available ,unsigned int attack_range) {

    this->type_game_object = type_name;
    this->defense = defense;
    this->attack = attack;
    this->health_max = health_max;
    this->is_static = is_static;
    this->is_available = is_available;
    this->attack_range = attack_range;

    this->healthBar = health_max;
}

Property::Property() {
    //default constructor
    this->type_game_object = "default";
    this->attack = 0;
    this->defense = 0;
    this->health_max = 10;
}

Property& Property::operator=(const state::Property& orig) {
    this->is_static = orig.is_static;
    this->healthBar = orig.healthBar;
    this->type_game_object = orig.type_game_object;
    this->attack = orig.attack;
    this->defense = orig.defense;
    this->health_max = orig.health_max;
    return *this;
}

unsigned int Property::getAttack() {
    return this->attack;
}

unsigned int Property::getDefense() {
    return this->defense;
}

unsigned int Property::getHealthMax() {
    return (unsigned int)this->health_max;
}

std::string Property::getStringType() {
    return type_game_object;
}

bool Property::setAvailability(bool is_available) {
    this->is_available = is_available;
    return true;
}

bool Property::getAvailability() {
    return this->is_available;
}

bool Property::isStatic (){
    return is_static;
}

bool Property::takeDamage (unsigned int damages){
    healthBar -= damages;
    return true;
}

bool Property::isAlive (){
    return healthBar>0;
}
bool Property::regenHealth (unsigned int health){
    if(healthBar <= 0)
        return false;
    healthBar += health;
    if(healthBar > health_max)
    {
        healthBar = health_max;
    }
    return true;
}

int Property::getHealth (){
    return healthBar;
}


