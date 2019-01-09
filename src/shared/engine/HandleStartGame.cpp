//
// Created by hikaru on 13/12/18.
//
#include <iostream>
#include "HandleStartGame.h"
#include "Engine.h"
#include "HandleCreation.h"
#include "HandleTurn.h"

using namespace engine;
using namespace std;
using namespace state;

HandleStartGame::HandleStartGame() = default;

HandleStartGame::~HandleStartGame() = default;

CommandTypeId HandleStartGame::getTypeId() const {
    return CommandTypeId::HANDLE_STARTGAME;
}

bool HandleStartGame::execute(state::State &state) {
    return true;
}

bool HandleStartGame::execute(state::State &state, engine::Engine &engine) {

    static int const terrain_int [] = {
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 0, 3, 3, 2, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 0, 1, 0, 3, 0, 3, 3, 0, 0, 1, 1, 1, 1, 2, 0,
            2, 0, 1, 0, 3, 0, 2, 3, 3, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 2, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1,
            2, 0, 1, 0, 3, 0, 3, 3, 3, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 0, 2, 3, 0, 0, 1, 1, 1, 1, 2, 0,
            0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    vector<int> test_terrain;
    for (int i = 0; i < 256; i++) {
        test_terrain.push_back(terrain_int[i]);
    }
    state.initializeMap(16,16,test_terrain);


    shared_ptr<engine::HandleCreation> create_starter1 (new engine::HandleCreation(7,5,farmer,false));
    engine.addCommands(create_starter1);
    shared_ptr<engine::HandleCreation> create_starter2 (new engine::HandleCreation(8,5,infantry,false));
    engine.addCommands(create_starter2);
    shared_ptr<engine::HandleCreation> create_starter3 (new engine::HandleCreation(6,5,archer,false));
    engine.addCommands(create_starter3);
    shared_ptr<engine::HandleCreation> create_town1 (new engine::HandleCreation(7,5,town,true));
    engine.addCommands(create_town1);
    cout << "test : created starters for the 1st npc" << endl;

    shared_ptr<engine::HandleTurn> end_turn1 (new engine::HandleTurn());
    engine.addCommands(end_turn1);


    shared_ptr<engine::HandleCreation> create_starter4 (new engine::HandleCreation(7,9,farmer,false));
    engine.addCommands(create_starter4);
    shared_ptr<engine::HandleCreation> create_starter5 (new engine::HandleCreation(8,9,infantry,false));
    engine.addCommands(create_starter5);
    shared_ptr<engine::HandleCreation> create_starter6 (new engine::HandleCreation(6,9,archer,false));
    engine.addCommands(create_starter6);
    shared_ptr<engine::HandleCreation> create_town2 (new engine::HandleCreation(7,9,town,true));
    engine.addCommands(create_town2);
    cout << "test : created starters for the 2nd npc" << endl;

    engine.addCommands(end_turn1);

    return true;
}

void HandleStartGame::serialize (Json::Value& out) const{

}
