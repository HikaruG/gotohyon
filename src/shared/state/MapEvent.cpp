//
// Created by ben on 14/11/18.
//

#include "state.h"
using namespace state;

MapEvent::~MapEvent () = default;
MapEvent::MapEvent (MapEventId map_event){
    this->map_event = map_event;
}

const EventTypeId MapEvent::getEventType (){
    return EventTypeId::MAP_EVENT;
}