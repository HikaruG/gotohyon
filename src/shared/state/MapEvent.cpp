//
// Created by ben on 14/11/18.
//

#include "state.h"
using namespace state;

MapEvent::~MapEvent () = default;
MapEvent::MapEvent (MapEventId map_event):Event(EventTypeId::MAP_EVENT){
    this->map_event = map_event;
}

EventTypeId MapEvent::getEventType () const{
    return EventTypeId::MAP_EVENT;
}