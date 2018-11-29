//
// Created by hikaru on 08/11/18.
//

#include "HandleDamage.h"
#include <iostream>

using namespace engine;
using namespace std;
CommandTypeId HandleDamage::getTypeId() const {
    return CommandTypeId::HANDLE_DAMAGE;
}

bool HandleDamage::execute(state::State& state, state::GameObject* object, state::GameObject* ennemy_object, state::Terrain &terrain) {
    ennemy_object->getProperty()->takeDamage(object->getProperty()->getAttack());
    if(!ennemy_object->getProperty()->isAlive()){
        cout << ennemy_object->getProperty()->getStringType() <<"destroyed" << endl;
        //commande delete
        if(ennemy_object->getProperty()->isStatic()){
            state::Building * ptr_destroyed_building = (state::Building * )ennemy_object;
            state.deleteBuilding(ptr_destroyed_building);
            //delete[] ptr_destroyed_building;
        }
        else{
            state::Unit * ptr_destroyed_unit = (state::Unit *)ennemy_object;
            state.deleteUnit(ptr_destroyed_unit);
            //delete[] ptr_destroyed_unit;
        }
        //delete[] ennemy_object;
    }
    return true;
}

bool HandleDamage::execute(state::State &state) {
    return true;
}

HandleDamage::HandleDamage() = default;

HandleDamage::~HandleDamage() = default;