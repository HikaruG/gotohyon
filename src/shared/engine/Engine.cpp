//
// Created by hikaru on 06/11/18.
//

#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace engine;
using namespace std;

Engine::Engine() {
}

Engine::~Engine() {

}


bool Engine::addCommands(shared_ptr<Command> command)
{
    shared_ptr<Command> new_command = command;
    list_commandes.push_back(new_command);
    return true;
}


bool Engine::update() {
    return true;
}