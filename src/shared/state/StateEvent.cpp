//
// Created by ben on 14/11/18.
//

#include "state.h"
using namespace state;


StateEvent::~StateEvent () = default;

StateEvent::StateEvent(StateEventId state_event):Event(EventTypeId::MAP_EVENT)
{
    this->state_event = state_event;
}

EventTypeId StateEvent::getEventType () const {
    return EventTypeId::STATE_EVENT;
}