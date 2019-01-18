//
// Created by ben on 07/01/19.
//
#include "HandleSelectedObject.h"

using namespace engine;


HandleSelectedObject::HandleSelectedObject(std::shared_ptr<state::GameObject> object) {
    this->selected_object=object;
}




CommandTypeId HandleSelectedObject::getTypeId() const {
    return CommandTypeId::HANDLE_SELECTEDOBJECT;
}

bool HandleSelectedObject::execute(state::State& state) {

    return true;
}


bool HandleSelectedObject::undo(state::State &state) {
    return true;
}


void HandleSelectedObject::serialize (Json::Value& out) const{
    out["CommandId"]=this->getTypeId();
}

HandleSelectedObject* HandleSelectedObject::deserialize (Json::Value& out){
    return this;
}

HandleSelectedObject::~HandleSelectedObject() = default;

HandleSelectedObject::HandleSelectedObject() = default;