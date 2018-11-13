//
// Created by hikaru on 07/11/18.
//
#include "LoadCommand.h"

using namespace engine;

CommandTypeId LoadCommand::getTypeId() const {
    return CommandTypeId::LOAD;
}

bool LoadCommand::execute(state::State &state) {
    return true;
}