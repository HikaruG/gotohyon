//
// Created by ben on 14/11/18.
//

#include "state.h"
using namespace state;


StateEvent::~StateEvent () = default;
StateEvent::StateEvent (StateEventId state_event){
    this->state_event = state_event;
}

const EventTypeId StateEvent::getEventType (){
    return EventTypeId::STATE_EVENT;
}