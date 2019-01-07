//
// Created by hikaru on 08/11/18.
//

#include "HandleDamage.h"
#include <iostream>

using namespace engine;
using namespace std;


HandleDamage::HandleDamage() = default;

HandleDamage::HandleDamage(state::Unit *selected_unit, state::GameObject *selected_target) {
    this->selected_unit = selected_unit;
    this->selected_target = selected_target;
}

HandleDamage::~HandleDamage() = default;


CommandTypeId HandleDamage::getTypeId() const {
    return CommandTypeId::HANDLE_DAMAGE;
}

bool HandleDamage::execute(state::State& state) {
    
    state::Position ennemy_position = this->selected_target->getPosition();
    state::Terrain * terrain = state.getMap().get()->getTerrain(ennemy_position.getX(), ennemy_position.getY()).get();

    cout << "player n" << this->selected_unit->getPlayerId() << " s " << this->selected_unit->getProperty()->getStringType() << " attacks ! " <<endl;
    this->selected_target->getProperty()->takeDamage(this->selected_unit->getProperty()->getAttack());
    cout << "player n" << this->selected_target->getPlayerId() << " s " << this->selected_target->getProperty()->getStringType() << " got hit for " << this->selected_unit->getProperty()->getAttack() <<"hp \n" <<endl;

    if(!this->selected_target->getProperty()->isAlive()){
        cout << "player n" << this->selected_target->getPlayerId() << " s " << this->selected_target->getProperty()->getStringType() <<"'s HP down to 0" << endl;
        //commande delete
        if(this->selected_target->getProperty()->isStatic()){
            //tue le player si sa centre ville est détruite
            if(this->selected_target->getProperty()->getStringType() == "town")
                state.setPlayerDead(selected_target->getPlayerId());

            //ensuite, détruit le batiment
            state::Building * ptr_destroyed_building = (state::Building * )this->selected_target;
            if(state.deleteBuilding(ptr_destroyed_building))
                cout << "building succesfully destroyed" << endl;
        }
        else{
            state::Unit * ptr_destroyed_unit = (state::Unit *)this->selected_target;
            if(state.deleteUnit(ptr_destroyed_unit))
                cout << "unit succesfully destroyed" << endl;
        }
    }

    this->selected_unit->getProperty()->setAvailability(false);
    return true;
}
