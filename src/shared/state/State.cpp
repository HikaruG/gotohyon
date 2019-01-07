#include <bits/unique_ptr.h>//
// Created by ben on 23/10/18.
//
#include <iostream>
#include "State.h"


using namespace state;
using namespace std;

State::State() {}

//constructeur de l'état qui prend en entrée le nombre de joueurs
State::State(unsigned int player_number,unsigned int npc_number)
        :game_map()

{
    this->player_nbr = player_number;
    this->remaining_players = player_number;
    this->day = 0;
    this->is_game_finished = false;
    initializePlayer(player_number, npc_number);
    cout << " construit " << this << endl;
}


//initialise la carte avec sa taille et sa carte de terrain en argument; la carte sera stockée en tant qu'attribut de state, de type unique_ptr
bool State::initializeMap (int size_x, int size_y, vector<int>& terrain){
    shared_ptr<Map> new_map (new Map(size_x,size_y,terrain));
    this->game_map = new_map;
    return true;
}

//instancie les joueurs et les npcs
bool State::initializePlayer(unsigned int player_count, unsigned int npc_count) {
    for(int i = 0; i < (int)player_count; i++){
        //création des joueurs
        if(i<((int)player_count - (int)npc_count)) {
            unique_ptr<Player> new_player(new Player(i, false));
            this->list_player.push_back(move(new_player));
        }
        //création des npcs
        else{
            unique_ptr<Player> new_player(new Player(i, true));
            this->list_player.push_back(move(new_player));
        }
    }
    this->current_player = list_player[0];
    return true;
}

bool State::addUnit(shared_ptr<Unit> unit) {
    //stock l'unité crée dans la liste list_game_object de Map
    game_map.get()->addGameObject(unit);

    //instanciation d'un pointeur pour pouvoir cast en shared ptr de building ou de unit
    shared_ptr<Unit> ptr_player_unit (unit);
    getCurrentPlayer().get()->addPlayerUnit(ptr_player_unit);
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return true;
}

//instancie les shared pointers des buildings sur la map: elle seront stockées dans 2 listes différentes

bool State::addBuilding(shared_ptr<Building> building) {
    //stock l'objet crée dans la liste list_game_object de Map
    game_map.get()->addGameObject(building);

    shared_ptr<Building> ptr_player_building = (building);
    getCurrentPlayer().get()->addPlayerBuilding(ptr_player_building);
    Event event = Event(EventTypeId::BUILDING_CHANGED);
    notifyObservers(event);
    return true;
}


bool State::deleteUnit(state::Unit* deleting_unit) {
    bool success = false;
    //recherche du play associé à l'unité détruite
    for(int i= 0; i < (int) player_nbr;i++){
        if(deleting_unit->getPlayerId() == list_player[i].get()->getPlayerId()) {
            success = list_player[i].get()->deletePlayerUnit(deleting_unit);
            cout << success << endl;
            break;
        }
    }
    //vérifie la suppression du pointeur dans la liste du joueur
    if(success) success = game_map.get()->deleteGameObject(deleting_unit);
    //retourne true si la suppression * de l'unité dans la liste de map a été effectuée
    cout << success << endl;
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return success;
}


bool State::deleteBuilding(state::Building* deleting_building) {
    bool success = false;
    //recherche du player associé au batiment détruit
    for(int i= 0; i < (int) player_nbr;i++){
        if(deleting_building->getPlayerId() == list_player[i].get()->getPlayerId()) {
            Player * attacked_player = list_player[i].get();
            success = attacked_player->deletePlayerBuilding(deleting_building);
            break;
        }
    }
    if(success) success = game_map.get()->deleteGameObject(deleting_building);
    Event event = Event(EventTypeId::BUILDING_CHANGED);
    notifyObservers(event);
    return success;
}

bool State::resetAvailability(){
    //pour le moment les batiments ne sont pas dispo
    /*
    for(int i =0; i < (int)this->current_player.get()->getPlayerBuildingList().size(); i++){
        this->current_player.get()->getPlayerBuildingList()[i].get()->getProperty()->setAvailability(true);
    }
    */
    for(int i =0; i < (int)this->current_player.get()->getPlayerUnitList().size(); i++){
        this->current_player.get()->getPlayerUnitList()[i].get()->getProperty()->setAvailability(true);
    }
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return true;
}

bool State::isGameFinished(){
    if(this->is_game_finished)
        return true;
    return false;
}

//obtient la référence de la carte initialisé au dessus
shared_ptr<Map> State::getMap (){
    return game_map;
}

//obtient le joueur actuel;
/// A FAIRE: update du player_id : soit une commande "fin du jour", soit ...

shared_ptr<Player>& State::getCurrentPlayer ()
{
    return this->current_player;
}

//renvoie le joueur actuel
unsigned  int State::getCurrentPlayerId() {
    return this->current_player_id;
}

//renvoie la liste des joueurs
vector<shared_ptr<Player> >& State::getListPlayer () {
    return this->list_player;
}

//renvoie le nombre de joueurs total
unsigned int State::getPlayerNbr(){
    return this->player_nbr;
}

//renvoie le jour actuel
unsigned int State::getDay()
{
    return day;
}

int State::getRemainingPlayers() {
    return this->remaining_players;
}

bool State::setPlayerDead(){
    this->current_player.get()->setIsDead();
    this->remaining_players--;
    if(remaining_players == 1)
        this->is_game_finished =true;
    return true;
}

//met à jour l'id de joueur courant après un fin de tour
bool State::setCurrentPlayerId() {
    if(this->current_player_id == this->player_nbr - 1){ //le player_id commence à 0 tandis que le player_nbr commence à 1
        this->current_player_id = 0;
        return true;
    }
    this->current_player_id ++;
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return true;
}

//met à jour le joueur courant : toujours executer après le setCurrentPlayerId
bool State::setCurrentPlayer() {
    this->current_player = list_player[this->current_player_id];
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return true;
}


//met à jour le nombre de "tour"
bool State::setDay() {
    if(this->current_player_id == 0) this->day ++;
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return true;
}

bool State::setPlayerNumber(unsigned int player_nbr) {
    this->player_nbr = player_nbr;
}

/*
bool State::setSelPosition(state::Position selected_position) {
    this->selected_position = selected_position;
    return true;
}

bool State::setSelUnit(std::shared_ptr<state::Unit> selected_unit) {
    this->selected_unit = selected_unit;
    return true;
}

bool State::setSelBuilding(std::shared_ptr<state::Building> selected_building) {
    this->selected_building = selected_building;
    return true;
}
*/

State::~State(){
    cout << " détruit " << this << endl;
}