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
}


Position GameObject::getPosition () {
    return this->object_position;
}
bool GameObject::setPosition (Position new_position) {
    if(this->getProperty()->isStatic())
    {
        return false;
    }
    this->object_position = new_position;
    return true;
}

unsigned int GameObject::getGame_object_id() const {
    return this->game_object_id;
}

unsigned int GameObject::getPlayerId() {
    return this->player_id;
}

 Property * GameObject::getProperty() {
    return &this->game_object_property;
}

void GameObject::setGame_object_id(unsigned int game_object_id) {
    this->game_object_id = game_object_id;
}

GameObject::GameObject(){

}

GameObject::~GameObject(){
    std::cout << " objet détruit " << this << std::endl;
}

void GameObject::setGame_object_id(unsigned int game_object_id){
    this->game_object_id = game_object_id;
}