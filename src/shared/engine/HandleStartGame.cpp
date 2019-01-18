//
// Created by hikaru on 13/12/18.
//
#include <iostream>
#include "HandleStartGame.h"
#include "Engine.h"
#include "HandleCreation.h"
#include "HandleTurn.h"
#include "SignalStartRecord.h"
#include "SignalStopRecord.h"

using namespace engine;
using namespace std;
using namespace state;

HandleStartGame::HandleStartGame(int player_count){
    this->player_count = player_count;
    this->starters_count = 3;
}


HandleStartGame::HandleStartGame(unsigned int player_nbr, unsigned int npc_count){
    this->player_count = player_nbr;
    this->npc_count = npc_count;
    this->starters_count = 3;
}

HandleStartGame::~HandleStartGame() = default;

CommandTypeId HandleStartGame::getTypeId() const {
    return CommandTypeId::HANDLE_STARTGAME;
}

bool HandleStartGame::execute(state::State &state) {
    return true;
}

bool HandleStartGame::execute(state::State &state, engine::Engine &engine) {
    state.initializePlayer(player_count, npc_count);

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
    shared_ptr<SignalStartRecord> start_rec (new SignalStartRecord());
    shared_ptr<SignalStopRecord> stop_rec (new SignalStopRecord());
    Json::Value thisCmd;
    this->serialize(thisCmd);
    engine.registerJsonCommand(thisCmd);
    engine.addCommands(stop_rec);
    switch(player_count) {
        case 2: {
            shared_ptr<engine::HandleCreation> create_starter1(new engine::HandleCreation(7, 5, farmer, false));
            engine.addCommands(create_starter1);
            shared_ptr<engine::HandleCreation> create_starter2(new engine::HandleCreation(8, 5, infantry, false));
            engine.addCommands(create_starter2);
            shared_ptr<engine::HandleCreation> create_starter3(new engine::HandleCreation(6, 5, archer, false));
            engine.addCommands(create_starter3);
            shared_ptr<engine::HandleCreation> create_town1(new engine::HandleCreation(7, 5, town, true));
            engine.addCommands(create_town1);
            cout << "test : created starters for the 1st npc" << endl;

            shared_ptr<engine::HandleTurn> end_turn1(new engine::HandleTurn());
            engine.addCommands(end_turn1);
            shared_ptr<engine::HandleCreation> create_starter4(new engine::HandleCreation(7, 9, farmer, false));
            engine.addCommands(create_starter4);
            shared_ptr<engine::HandleCreation> create_starter5(new engine::HandleCreation(8, 9, infantry, false));
            engine.addCommands(create_starter5);
            shared_ptr<engine::HandleCreation> create_starter6(new engine::HandleCreation(6, 9, archer, false));
            engine.addCommands(create_starter6);
            shared_ptr<engine::HandleCreation> create_town2(new engine::HandleCreation(7, 9, town, true));
            engine.addCommands(create_town2);
            cout << "test : created starters for the 2nd npc" << endl;
            engine.addCommands(end_turn1);
            break;
        }
        case 3: {
            shared_ptr<engine::HandleCreation> create3_starter1(new engine::HandleCreation(7, 5, farmer, false));
            engine.addCommands(create3_starter1);
            shared_ptr<engine::HandleCreation> create3_starter2(new engine::HandleCreation(8, 5, infantry, false));
            engine.addCommands(create3_starter2);
            shared_ptr<engine::HandleCreation> create3_starter3(new engine::HandleCreation(6, 5, archer, false));
            engine.addCommands(create3_starter3);
            shared_ptr<engine::HandleCreation> create3_town1(new engine::HandleCreation(7, 5, town, true));
            engine.addCommands(create3_town1);
            cout << "test : created starters for the 1st player" << endl;

            shared_ptr<engine::HandleTurn> end3_turn1(new engine::HandleTurn());
            engine.addCommands(end3_turn1);

            shared_ptr<engine::HandleCreation> create3_starter4(new engine::HandleCreation(7, 9, farmer, false));
            engine.addCommands(create3_starter4);
            shared_ptr<engine::HandleCreation> create3_starter5(new engine::HandleCreation(8, 9, infantry, false));
            engine.addCommands(create3_starter5);
            shared_ptr<engine::HandleCreation> create3_starter6(new engine::HandleCreation(6, 9, archer, false));
            engine.addCommands(create3_starter6);
            shared_ptr<engine::HandleCreation> create3_town2(new engine::HandleCreation(7, 9, town, true));
            engine.addCommands(create3_town2);
            cout << "test : created starters for the 1st npc" << endl;
            engine.addCommands(end3_turn1);

            shared_ptr<engine::HandleCreation> create3_starter7(new engine::HandleCreation(4, 7, farmer, false));
            engine.addCommands(create3_starter7);
            shared_ptr<engine::HandleCreation> create3_starter8(new engine::HandleCreation(4, 8, infantry, false));
            engine.addCommands(create3_starter8);
            shared_ptr<engine::HandleCreation> create3_starter9(new engine::HandleCreation(4, 6, archer, false));
            engine.addCommands(create3_starter9);
            shared_ptr<engine::HandleCreation> create3_town3(new engine::HandleCreation(4, 7, town, true));
            engine.addCommands(create3_town3);
            cout << "test : created starters for the 2nd npc" << endl;
            engine.addCommands(end3_turn1);
            break;
        }
        case 4:{
            shared_ptr<engine::HandleCreation> create3_starter1(new engine::HandleCreation(7, 3, farmer, false));
            engine.addCommands(create3_starter1);
            shared_ptr<engine::HandleCreation> create3_starter2(new engine::HandleCreation(8, 3, infantry, false));
            engine.addCommands(create3_starter2);
            shared_ptr<engine::HandleCreation> create3_starter3(new engine::HandleCreation(6, 3, archer, false));
            engine.addCommands(create3_starter3);
            shared_ptr<engine::HandleCreation> create3_town1(new engine::HandleCreation(7, 3, town, true));
            engine.addCommands(create3_town1);
            cout << "test : created starters for the 1st player" << endl;
            shared_ptr<engine::HandleTurn> end3_turn1(new engine::HandleTurn());
            engine.addCommands(end3_turn1);

            shared_ptr<engine::HandleCreation> create3_starter4(new engine::HandleCreation(7, 11, farmer, false));
            engine.addCommands(create3_starter4);
            shared_ptr<engine::HandleCreation> create3_starter5(new engine::HandleCreation(8, 11, infantry, false));
            engine.addCommands(create3_starter5);
            shared_ptr<engine::HandleCreation> create3_starter6(new engine::HandleCreation(6, 11, archer, false));
            engine.addCommands(create3_starter6);
            shared_ptr<engine::HandleCreation> create3_town2(new engine::HandleCreation(7, 11, town, true));
            engine.addCommands(create3_town2);
            cout << "test : created starters for the 1st npc" << endl;
            engine.addCommands(end3_turn1);

            shared_ptr<engine::HandleCreation> create3_starter7(new engine::HandleCreation(3, 7, farmer, false));
            engine.addCommands(create3_starter7);
            shared_ptr<engine::HandleCreation> create3_starter8(new engine::HandleCreation(3, 8, infantry, false));
            engine.addCommands(create3_starter8);
            shared_ptr<engine::HandleCreation> create3_starter9(new engine::HandleCreation(3, 6, archer, false));
            engine.addCommands(create3_starter9);
            shared_ptr<engine::HandleCreation> create3_town3(new engine::HandleCreation(3, 7, town, true));
            engine.addCommands(create3_town3);
            cout << "test : created starters for the 2nd npc" << endl;
            engine.addCommands(end3_turn1);

            shared_ptr<engine::HandleCreation> create4_starter10(new engine::HandleCreation(9, 7, farmer, false));
            engine.addCommands(create4_starter10);
            shared_ptr<engine::HandleCreation> create4_starter11(new engine::HandleCreation(9, 8, infantry, false));
            engine.addCommands(create4_starter11);
            shared_ptr<engine::HandleCreation> create4_starter12(new engine::HandleCreation(9, 6, archer, false));
            engine.addCommands(create4_starter12);
            shared_ptr<engine::HandleCreation> create4_town4(new engine::HandleCreation(9, 7, town, true));
            engine.addCommands(create4_town4);
            cout << "test : created starters for the 3rd npc" << endl;
            engine.addCommands(end3_turn1);
            break;
        }
        default:
            return false;
    }
    engine.addCommands(start_rec);
    return true;
}


bool HandleStartGame::undo(state::State &state) {
    for(shared_ptr<GameObject> objects : state.getMap().get()->getListGameObject()){
        if(objects.get()->getProperty()->isStatic())
            state.deleteBuilding((Building *) objects.get(),false);
        else
            state.deleteUnit((Unit *) objects.get(),false);
    }
    return true;
}


void HandleStartGame::serialize (Json::Value& out) const{
    out["CommandId"]=this->getTypeId();
    out["s_count"]=this->starters_count;
    out["p_count"]=this->player_count;
    out["npc_count"]=this->npc_count;
}

HandleStartGame* HandleStartGame::deserialize (Json::Value& out){
    this->starters_count=out.get("s_count",0).asInt();
    this->player_count=out.get("p_count",0).asUInt();
    this->npc_count=out.get("npc_count",0).asUInt();
    return this;
}
