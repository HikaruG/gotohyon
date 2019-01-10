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

LoadCommand::LoadCommand() = default;

LoadCommand::~LoadCommand() = default;

void LoadCommand::serialize (Json::Value& out) const{
    out["CommandId"]=2;
}

LoadCommand* LoadCommand::deserialize (Json::Value& out){
    return this;
}