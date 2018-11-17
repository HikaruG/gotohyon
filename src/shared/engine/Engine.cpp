//
// Created by hikaru on 06/11/18.
//

#include "Engine.h"
#include <SFML/Graphics.hpp>
using namespace engine;

Engine::Engine() {}

Engine::~Engine() {

}


bool Engine::addCommands(Command* command)
{
    //current_command.push_back(*command);
    return true;
}


bool Engine::update() {
    return true;
}