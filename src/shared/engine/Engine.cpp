//
// Created by hikaru on 06/11/18.
//

#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Command.h"
#include <assert.h>

using namespace engine;
using namespace std;

//fonction permettant de supprimer le premier élément de la liste
void pop_front(vector<shared_ptr<Command>> & v)
{
    assert(!v.empty());
    v.erase(v.begin());
}

Engine::Engine():record(Json::Value(Json::nullValue)) {
}

Engine::~Engine() {
}

bool Engine::execute(state::State & state) {
    int list_size = list_commands.size();
    unsigned int food, gold;
    if (list_size == 0)
        return true;

    while (list_commands.size() != 0) {
        switch (list_commands.front().get()->getTypeId()) {
            case HANDLE_STARTGAME:
                list_commands.front().get()->execute(state);
            case HANDLE_GROWTH:
                list_commands.front().get()->execute(state);
                state.getCurrentPlayer().get()->getRessource(food, gold);
                cout << "player" << state.getCurrentPlayer().get()->getPlayerId() << "'s current gold is " << gold << endl;
                cout << "player" << state.getCurrentPlayer().get()->getPlayerId() << "'s current food is " << food << endl;
                break;
            case HANDLE_MOVEMENT:
                list_commands.front().get()->execute(state);
                break;
            case HANDLE_CANATTACK:
                list_commands.front().get()->execute(state);
                break;
            case HANDLE_DAMAGE:
                list_commands.front().get()->execute(state);
                break;
            case HANDLE_CREATION:
                list_commands.front().get()->execute(state);
                state.getCurrentPlayer().get()->getRessource(food, gold);
                cout << "player" << state.getCurrentPlayer().get()->getPlayerId() << "'s current gold is " << gold << endl;
                cout << "player" << state.getCurrentPlayer().get()->getPlayerId() << "'s current food is " << food << endl;
                break;
            case HANDLE_TURN:
                if(!list_commands.front().get()->execute(state))
                    return true;
                break;
            case HANDLE_ENDGAME:
                list_commands.front().get()->execute(state);
                list_commands.clear();
                return true;
            default:
                return false;
        }
        executed_commands.push_back(list_commands.front());
        pop_front(list_commands);
        if (list_size != (int)list_commands.size() + 1)
            return false;
        list_size = list_commands.size();
    }

    return true;
}

bool Engine::addCommands(shared_ptr<Command> command)
{
    list_commands.push_back(command);
    return true;
}

bool Engine::undo(state::State& state) {
    int list_size = executed_commands.size();
    unsigned int food, gold;
    if (list_size == 0)
        return true;

    while (executed_commands.size() != 0) {
        executed_commands.front().get()->undo(state);
    pop_front(executed_commands);
    if (list_size != (int)executed_commands.size() + 1)
        return false;
    list_size = executed_commands.size();
    }
}

bool Engine::update() {
    return true;
}