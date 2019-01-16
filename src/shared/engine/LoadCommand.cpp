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


bool LoadCommand::undo(state::State &state) {
    return true;
}


LoadCommand::LoadCommand() = default;

LoadCommand::~LoadCommand() = default;

void LoadCommand::serialize (Json::Value& out) const{
    out["CommandId"]=this->getTypeId();
    out["String"]=this->file_name;
}

LoadCommand* LoadCommand::deserialize (Json::Value& out){
    this->file_name=out.get("String","error").asString();
    return this;
}