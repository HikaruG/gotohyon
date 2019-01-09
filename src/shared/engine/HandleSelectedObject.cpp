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
