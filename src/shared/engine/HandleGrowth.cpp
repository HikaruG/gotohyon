//
// Created by hikaru on 07/11/18.
//
#include "HandleGrowth.h"

using namespace engine;

CommandTypeId HandleGrowth::getTypeId() const {
    return CommandTypeId::HANDLE_GROWTH;
}

bool HandleGrowth::execute(unsigned int *food, unsigned int *gold, state::State &state) {
    
}