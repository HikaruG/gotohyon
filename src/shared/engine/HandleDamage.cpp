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

bool HandleDamage::execute(state::State& state ) {

    state::GameObject * ennemy_object = state.getSelTarget().get();
    state::Position ennemy_position = ennemy_object->getPosition();
    state::Terrain * terrain = state.getMap().get()->getTerrain(ennemy_position.getX(), ennemy_position.getY()).get();

    cout << "player n" << state.getSelUnit().getPlayerId() << " s " << state.getSelUnit().getProperty()->getStringType() << " attacks ! " <<endl;
    ennemy_object->getProperty()->takeDamage(state.getSelUnit().getProperty()->getAttack());

    cout << "player n" << ennemy_object->getPlayerId() << " s " << ennemy_object->getProperty()->getStringType() << " got hit for " << state.getSelUnit().getProperty()->getAttack() <<"hp \n" <<endl;
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
    state.getSelUnit().getProperty()->setAvailability(false);
    return true;
}

HandleDamage::HandleDamage() = default;

HandleDamage::~HandleDamage() = default;