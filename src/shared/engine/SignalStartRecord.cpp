//
// Created by ben on 16/01/19.
//

#include "SignalStartRecord.h"

using namespace engine;

SignalStartRecord::SignalStartRecord () = default;
CommandTypeId SignalStartRecord::getTypeId () const{
    return SIG_STARTRECORD;
}

bool SignalStartRecord::execute (state::State& state)
{
    return true;
}
bool SignalStartRecord::undo (state::State& state){
    return true;
}
void SignalStartRecord::serialize (Json::Value& out) const{

}