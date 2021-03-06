//
// Created by hikaru on 08/11/18.
//

#include "HandleDamage.h"
#include "HandleCreation.h"
#include <iostream>


using namespace engine;
using namespace std;


HandleDamage::HandleDamage() = default;

HandleDamage::HandleDamage(state::Unit *selected_unit, state::GameObject *selected_target) {
    this->selected_unit = selected_unit;
    this->selected_unit_id = selected_unit->getGame_object_id();
    this->selected_target = selected_target;
    this->selected_target_id = selected_target->getGame_object_id();
    this->target_x = selected_target->getPosition().getX();
    this->target_y = selected_target->getPosition().getY();
    this->target_hp = selected_target->getProperty()->getHealth();
    this->target_static = selected_target->getProperty()->isStatic();
    if(target_static) {
        state::Building *building_targeted = (state::Building *) selected_target;
        this->target_type = building_targeted->getBuildingType();
    }
    else {
        state::Unit *unit_targeted = (state::Unit *) selected_target;
        this->target_type = unit_targeted->getUnitType();
    }
}

HandleDamage::~HandleDamage() = default;

CommandTypeId HandleDamage::getTypeId() const {
    return CommandTypeId::HANDLE_DAMAGE;
}

bool HandleDamage::execute(state::State& state) {

    if(!this->selected_unit)
    {
        this->selected_unit = (state::Unit *) state.getGameObject(this->selected_unit_id).get();
        cout << " selected unit is " << this->selected_unit->getProperty()->getStringType() << endl;
        if(!this->selected_unit)
            throw std::invalid_argument("selected unit not found");
    }
    if(!this->selected_target)
    {
        this->selected_target = state.getGameObject(selected_target_id).get();
        cout << " selected target is " << this->selected_target->getProperty()->getStringType() << endl;
        if(!this->selected_target)
            throw std::invalid_argument("selected target not found");
    }

    state::Position ennemy_position = this->selected_target->getPosition();
    //state::Terrain * terrain = state.getMap().get()->getTerrain(ennemy_position.getX(), ennemy_position.getY()).get();

    cout << "player n" << this->selected_unit->getPlayerId() << " s " << this->selected_unit->getProperty()->getStringType() << " whose id is " << this->selected_unit->getGame_object_id()  << " attacks ! " <<endl;
    this->selected_target->getProperty()->takeDamage(this->selected_unit->getProperty()->getAttack());
    cout << "player n" << this->selected_target->getPlayerId() << " s " << this->selected_target->getProperty()->getStringType()  << " whose id is " << this->selected_target->getGame_object_id() << " got hit for " << this->selected_unit->getProperty()->getAttack() <<"hp \n" <<endl;

    if(!this->selected_target->getProperty()->isAlive()){
        cout << "player n" << this->selected_target->getPlayerId() << " s " << this->selected_target->getProperty()->getStringType() <<"'s HP down to 0" << endl;
        //commande delete
        if(this->selected_target->getProperty()->isStatic()){
            //tue le player si sa centre ville est détruite
            if(this->selected_target->getProperty()->getStringType() == "town") {
                state.setPlayerDead(selected_target->getPlayerId());
            }

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


bool HandleDamage::undo(state::State &state) {
    if (!this->selected_target) {
        cout<<"selected traget is empty"<<endl;
        this->selected_target = state.getGameObject(selected_target_id).get();
    }
    if (this->selected_target) {
        cout<<"selected target found : hp left is "<< selected_target->getProperty()->getHealth() << endl;
        if(this->selected_target->getProperty()->getHealth() <= 0) {
            cout<<" reviving " << selected_target->getProperty()->getStringType() << endl;
            state.reviveGameObject(selected_target_id, selected_target->getPlayerId(), selected_target->getProperty()->isStatic());

            if(this->selected_target->getProperty()->getStringType() == "town"){
                state.setPlayerAlive(selected_target_id);
            }
        }
        state.getGameObject(selected_target_id)->getProperty()->regenHealth(this->selected_unit->getProperty()->getAttack());
        cout<<"regenerated wounded unit : "<<state.getGameObject(selected_unit_id)->getProperty()->getAttack()<<endl;
        return true;
    } else{
        if(state.reviveGameObject(selected_target_id, selected_target->getPlayerId(), selected_target->getProperty()->isStatic()))
        {
            this->selected_target = state.getGameObject(selected_target_id).get();
            state.getGameObject(selected_target_id)->getProperty()->regenHealth(this->selected_unit->getProperty()->getAttack());
            cout<<"regenerated : "<<this->selected_unit->getProperty()->getAttack()<<endl;
            if(this->selected_target->getProperty()->getStringType() == "town"){
                state.setPlayerAlive(selected_target_id);
            }
        }
        return false;
    }
}

void HandleDamage::serialize (Json::Value& out) const{
    out["CommandId"] = this->getTypeId();
    out["selected_unit_id"] = this->selected_unit->getGame_object_id();
    out["selected_target_id"] = this->selected_target->getGame_object_id();
    out["target_x"] = this->target_x;
    out["target_y"] = this->target_y;
    out["target_hp"] = this->target_hp;
}

HandleDamage* HandleDamage::deserialize (Json::Value& out){
    this->selected_target = nullptr;
    this->selected_unit = nullptr;
    this->selected_unit_id = out.get("selected_unit_id",0).asUInt();
    this->selected_target_id = out.get("selected_target_id",0).asUInt();
    this->target_x = out.get("target_x",0).asUInt();
    this->target_y = out.get("target_y",0).asUInt();
    this->target_hp = out.get("target_hp",0).asUInt();

    return this;

}