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
    cout << "player n" << object->getPlayerId() << " s " << object->getProperty()->getStringType() << " attacks ! " <<endl;
    ennemy_object->getProperty()->takeDamage(object->getProperty()->getAttack());

    cout << "player n" << ennemy_object->getPlayerId() << " s " << ennemy_object->getProperty()->getStringType() << " got hit for " << object->getProperty()->getAttack() <<"hp \n" <<endl;
    if(!ennemy_object->getProperty()->isAlive()){
        //commande delete
        if(ennemy_object->getProperty()->isStatic()){
            //tue le player si sa centre ville est détruite
            if(ennemy_object->getProperty()->getStringType() == "town")
                state.setPlayerDead();
            //ensuite, détruit le batiment
            state::Building * ptr_destroyed_building = (state::Building * )ennemy_object;
            if(state.deleteBuilding(ptr_destroyed_building))
                cout << "player n" << ennemy_object->getPlayerId() << " s " << ennemy_object->getProperty()->getStringType() <<" got destroyed" << endl;
        }
        else{
            state::Unit * ptr_destroyed_unit = (state::Unit *)ennemy_object;
            if(state.deleteUnit(ptr_destroyed_unit))
                cout << "player n" << ennemy_object->getPlayerId() << " s " << ennemy_object->getProperty()->getStringType() <<" got destroyed" << endl;

        }
    }
    object->getProperty()->setAvailability(false);
    return true;
}

bool HandleDamage::execute(state::State &state) {
    return true;
}

HandleDamage::HandleDamage() = default;

HandleDamage::~HandleDamage() = default;