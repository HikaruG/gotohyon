#include <bits/unique_ptr.h>//
// Created by ben on 23/10/18.
//
#include <iostream>
#include "State.h"


using namespace state;
using namespace std;

//initialise la carte avec sa taille et sa carte de terrain en argument; la carte sera stockée en tant qu'attribut de state, de type unique_ptr
bool State::initializeMap (int size_x, int size_y, vector<int>& terrain){
    shared_ptr<Map> new_map (new Map(size_x,size_y,terrain));
    this->game_map = new_map;
    return true;
}

//obtient la référence de la carte initialisé au dessus
shared_ptr<Map> State::getMap (){
    return game_map;
}

//constructeur de l'état qui prend en entrée le nombre de joueurs
State::State(unsigned int player_number)
      :game_map()

{
    this->player_nbr = player_number;
    day_count = 0;
    cout << " construit " << this << endl;
}

//instancie un joueur, il faudra faire appel à cette méthode autant de fois que de joueurs
bool State::initializePlayer() {
    unique_ptr<Player> new_player (new Player());
    this->list_player.push_back( move(new_player) );
    return true;
}


unsigned  int State::getCurrentPlayerId() {
    return this->current_player_id;
}

bool State::setCurrentPlayerId(unsigned int player_id) {
    this->current_player_id = player_id;
    return true;
}

//obtient le joueur actuel;
/// A FAIRE: update du player_id : soit une commande "fin du jour", soit ...
unique_ptr<Player>& State::getCurrentPlayer (unsigned int player_id)
{
    for(int i = 0; i < (int) this->player_nbr; i++){
        if(list_player[i].get()->getPlayerId() == player_id){
            return list_player[i];
        }
    }
    throw invalid_argument(" can't find the player ! Mayday");
}

//met à jour le nombre de "tour"
bool State::setDay(unsigned int day) {
    this->day_count = day;
    return true;
}

unsigned int State::getDayCount()
{
    return day_count;
}

//instancie les shared pointers des unités sur la map: elle seront stockées dans 2 listes différentes
bool State::addUnit(Position position, shared_ptr<Unit> unit) {
    //stock l'unité crée dans la liste list_game_object de Map
    game_map.get()->addGameObject(game_map.get()->getListGameObject(),unit);

    //instanciation d'un pointeur pour pouvoir cast en shared ptr de building ou de unit
    shared_ptr<Unit> ptr_player_unit (unit);
    getCurrentPlayer(unit.get()->getPlayerId()).get()->addPlayerUnit(ptr_player_unit);

}

//instancie les shared pointers des buildings sur la map: elle seront stockées dans 2 listes différentes
bool State::addBuilding(Position position, shared_ptr<Building> building) {

    //stock l'objet crée dans la liste list_game_object de Map
    game_map.get()->addGameObject(game_map.get()->getListGameObject(),building);

    shared_ptr<Building> ptr_player_building = (building);
    //super long ... ça prend le joueur actuel et lui met le
    getCurrentPlayer(building.get()->getPlayerId()).get()->addPlayerBuilding(ptr_player_building);
    return true;
}

State::~State(){
    cout << " détruit " << this << endl;
}