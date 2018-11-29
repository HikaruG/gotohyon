//
// Created by ben on 14/11/18.
//

#include "Event.h"

using namespace state;

Event::~Event () = default;

Event::Event(state::EventTypeId event_type) {
    this->event_type = event_type;
}

EventTypeId Event::getEventType () const
{
    return event_type;
}
