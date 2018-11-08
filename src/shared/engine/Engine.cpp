//
// Created by hikaru on 06/11/18.
//

#include "Engine.h"

using namespace engine;

Engine::Engine()
:current_state(state::State(1))
{

}

const state::State& Engine::getState() const
{
    return current_state;
}


state::State addCommands(Command* command)
{
    
}