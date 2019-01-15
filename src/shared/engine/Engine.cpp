//
// Created by hikaru on 06/11/18.
//

#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Command.h"
#include "HandleStartGame.h"
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
    record["commands"] = Json::arrayValue;
}

Engine::~Engine() {
}

bool Engine::execute(state::State & state) {
    int list_size = list_commands.size();
    unsigned int food, gold;
    if (list_size == 0)
        return true;

    while (list_commands.size() != 0) {
        list_size = list_commands.size();
        Json::Value thisCmd;
        switch (list_commands.front().get()->getTypeId()) {
            case HANDLE_STARTGAME:
                {
                list_commands.front().get()->serialize(thisCmd);
                HandleStartGame *front_cmd = (HandleStartGame *) list_commands.front().get();
                front_cmd->execute(state, *this);
            }
            case HANDLE_GROWTH:
                list_commands.front().get()->serialize(thisCmd);
                list_commands.front().get()->execute(state);
                state.getCurrentPlayer().get()->getRessource(food, gold);
                cout << "player" << state.getCurrentPlayer().get()->getPlayerId() << "'s current gold is " << gold << endl;
                cout << "player" << state.getCurrentPlayer().get()->getPlayerId() << "'s current food is " << food << endl;
                break;
            case HANDLE_MOVEMENT:
                list_commands.front().get()->serialize(thisCmd);
                list_commands.front().get()->execute(state);
                break;
            case HANDLE_CANATTACK:
                list_commands.front().get()->serialize(thisCmd);
                list_commands.front().get()->execute(state);
                break;
            case HANDLE_DAMAGE:
                list_commands.front().get()->serialize(thisCmd);
                list_commands.front().get()->execute(state);
                break;
            case HANDLE_CREATION:
                list_commands.front().get()->serialize(thisCmd);
                list_commands.front().get()->execute(state);
                state.getCurrentPlayer().get()->getRessource(food, gold);
                break;
            case HANDLE_TURN:
                list_commands.front().get()->serialize(thisCmd);
                if(!list_commands.front().get()->execute(state))
                    return false;
                break;
            case HANDLE_ENDGAME:
                list_commands.front().get()->serialize(thisCmd);
                list_commands.front().get()->execute(state);
                list_commands.clear();
                return false;
            case SIG_STARTRECORD:
                intern_record = false;
            default:
                throw invalid_argument(" can't find the command, aborting !");
        }
        sf::Time client_engineTime = sf::milliseconds(50);
        sf::sleep(client_engineTime);

        //add serialised command to array
        if(list_commands.front().get()->getTypeId() == HANDLE_STARTGAME)
            list_size = list_commands.size();
        executed_commands.push_back(list_commands.front());
        pop_front(list_commands);
        /* cette méthode de vérification ne fonctionne plus en multi threads
        if (list_size != (int)list_commands.size() + 1) {
            cout << "la variable list size " << list_size << " et la size de la liste " << list_commands.size() << endl;
            throw invalid_argument(" error executing the command, aborting !");
        }
        */
        list_size = list_commands.size();
        record["commands"].append(thisCmd);
        //cout<<"###JSON###\n"<<record<<"\n###END JSON###"<<endl;
    }
    return true;
}

bool Engine::addCommands(shared_ptr<Command> command)
{
    list_commands.push_back(command);
    return true;
}

bool Engine::undo(state::State& state) {
    cout << "---------------- deepAI thinking twice ----------------" << endl;
    cout << "----------------       rollback        ----------------" << endl;
    int list_size = executed_commands.size();
    unsigned int food, gold;
    if (list_size == 0)
        return true;

    while (executed_commands.size() != 0) {
        if(!executed_commands.front().get()->undo(state))
            return false;
    pop_front(executed_commands);
    if (list_size != (int)executed_commands.size() + 1)
        throw invalid_argument(" error undo-ing the command, aborting !");
    list_size = executed_commands.size();
    }
}

bool Engine::cleanExecuted() {
    this->executed_commands.clear();
}

bool Engine::update() {
    return true;
}