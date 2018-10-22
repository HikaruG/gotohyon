//
// Created by ben on 15/10/18.
//
#include "GameObject.h"
#include <iostream>

using namespace state;

GameObject::GameObject(unsigned int gameobject_id, unsigned int player_ids, state::Position pos, state::Property property)
{
    this->game_object_id = gameobject_id;
    this->game_object_property = property;
    this->player_id = player_ids;
    this->object_position = pos;

    this->health_bar = this->game_object_property.get_health_max();
    this->is_destroyed = false;
}


bool GameObject::takeDamage (int damage)
{
    this->health_bar -= damage;
    if (this->health_bar <= 0)
    {
        this->is_destroyed = true;
    }
    return true;
}

Position GameObject::getPosition () {
    return this->object_position;
}
bool GameObject::setPosition (Position new_position) {
    if(this->is_static)
    {
        return false;
    }
    this->object_position = new_position;
    return true;
}

unsigned int GameObject::getGame_object_id() const {
    return this->game_object_id;
}

bool GameObject::getHealth(int *health) {
    *health = health_bar;
    return true;
}