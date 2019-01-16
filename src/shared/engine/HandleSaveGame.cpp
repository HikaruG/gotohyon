//
// Created by ben on 15/01/19.
//

#include "HandleSaveGame.h"
#include <iostream>
#include <fstream>

using namespace engine;

HandleSaveGame::HandleSaveGame () = default;

CommandTypeId HandleSaveGame::getTypeId () const{
    return HANDLE_SAVEGAME;
}

bool HandleSaveGame::execute (state::State& state){
    return false;
}
bool HandleSaveGame::undo (state::State& state){
    return true;
}

bool HandleSaveGame::execute (Json::Value& json_file){

    std::cout<<"saving game ... ";
    std::ofstream new_save;
    new_save.open ("res/replay.json", std::ios::out);
    if (new_save.is_open()) {
        Json::StyledWriter styledWriter;
        new_save << styledWriter.write(json_file);
        new_save.close();
        std::cout<<" done"<<std::endl;
        return true;
    }
    std::cout<<" error ! not saved : file not open"<<std::endl;
    return false;
}

void HandleSaveGame::serialize(Json::Value &out) const {
    return;
}