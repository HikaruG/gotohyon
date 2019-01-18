//
// Created by ben on 16/01/19.
//
#include "SignalStopRecord.h"

using namespace engine;

SignalStopRecord::SignalStopRecord () = default;
CommandTypeId SignalStopRecord::getTypeId () const{
    return SIG_STOPRECORD;
}

bool SignalStopRecord::execute (state::State& state)
{
    return true;
}
bool SignalStopRecord::undo (state::State& state){
    return true;
}
void SignalStopRecord::serialize (Json::Value& out) const{

}
