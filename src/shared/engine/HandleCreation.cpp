//
// Created by hikaru on 07/11/18.
//

#include <iostream>
#include "HandleCreation.h"

using namespace engine;

CommandTypeId HandleCreation::getTypeId() const {
    return CommandTypeId::HANDLE_CREATION;
}

bool HandleCreation::execute(state::State &state, unsigned int pos_x, unsigned int pos_y, int type, bool is_static) {

    state::Map * map;
    state::Terrain * terrain = nullptr;
    state::Position position(pos_x, pos_y);

    map = state.getMap();
    map->getTerrain(pos_x,pos_y,terrain);
    if(terrain->getTerrainType() == state::water){
        std::cout << "impossible de créer l'objet" << std::endl;
        return true;
    }

    if(is_static){
        if(type > 5) //il n' y a que 5 batiments
        {
            std::cout << "impossible de créer le batiment: n'existe pas " << std::endl;
            return true;
        }

    }
    else{
        if(type > 3)
        {
            std::cout << "impossible de créer l'unité : n'existe pas" << std::endl;
            return true;
        }
    }

    state.addGameObject(state.getCurrentPlayer()->getPlayerId(),position, is_static, type);
    return true;
}

HandleCreation::HandleCreation()
{

}

CommandTypeId Command::getTypeId() const {
    CommandTypeId creation = 
    return creation;
}













