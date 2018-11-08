//
// Created by hikaru on 06/11/18.
//

#include "Engine.h"

using namespace engine;

Engine::Engine()
:current_state(state::State(1))
{

}

Engine::~Engine() {

}

const state::State& Engine::getState() const
{
    return current_state;
}

bool Engine::addCommands(Command* command)
{
    //current_command.push_back(*command);
    return true;
}

bool Engine::setState(state::State& new_state) {
    current_state = new_state;
    return true;
}

bool Engine::update() {
    return true;
}