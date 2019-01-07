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


Engine::Engine() {
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
            case HANDLE_GROWTH:
                list_commands.front().get()->execute(state);
                state.getCurrentPlayer().get()->getRessource(food, gold);
                cout << "player" << state.getCurrentPlayer().get()->getPlayerId() << "'s current gold is " << gold << endl;
                cout << "player" << state.getCurrentPlayer().get()->getPlayerId() << "'s current food is " << food << endl;
                break;
            case HANDLE_MOVEMENT:
                list_commands.front().get()->execute(state);
                break;
            case HANDLE_ATTACK:
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
                list_commands.front().get()->execute(state);
                break;
            case HANDLE_ENDGAME:
                list_commands.front().get()->execute(state);
                break;
            default:
                return false;
        }
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


bool Engine::update() {
    return true;
}