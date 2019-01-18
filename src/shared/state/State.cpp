#include <bits/unique_ptr.h>//
// Created by ben on 23/10/18.
//
#include <iostream>
#include "State.h"


using namespace state;
using namespace std;

State::State() {
    this->day = 0;
    this->object_count = 0;
}

//constructeur de l'état qui prend en entrée le nombre de joueurs
State::State(unsigned int player_number,unsigned int npc_number)
        :game_map()

{
    this->player_nbr = player_number;
    this->remaining_players = player_number;
    this->day = 0;
    this->object_count = 0;
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
    for(unsigned int i = 0; i < player_count; i++){
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
    this->player_nbr = player_count;
    this->remaining_players = player_count;
    this->current_player_id = 0;
    this->current_player = list_player[current_player_id];
    return true;
}

bool State::killPlayer(unsigned int player_id) {
    int player_index = -1;

    for(int i = 0; i < list_player.size(); i++) {
        if (list_player[i].get()->getPlayerId() == player_id) {
            player_index = i;
            break;
        }
    }
    if(player_index == -1){
        cout << "unexpected index for the deleting player" << endl;
        return false;
    }
    int map_list_size = game_map.get()->getListGameObject().size();
    int j = 0;
    while(j < map_list_size) {
        GameObject *object_i = game_map.get()->getListGameObject()[j].get();
        if(object_i->getPlayerId() == player_id){
            game_map.get()->deleteGameObject(object_i);
            if(object_i->getProperty()->isStatic())// utile seulement pour le record à l'envers, lors d'un mode de jeu supérieur à 2 players
                list_player[player_index].get()->deletePlayerBuilding((Building *)object_i);
            else
                list_player[player_index].get()->deletePlayerUnit((Unit *)object_i);
        }
        else
            j++;
        map_list_size = game_map.get()->getListGameObject().size();
    }
    if(list_player[player_index].get()->getPlayerUnitList().size())
        cout << "error, the size of unit list is not 0 but " <<list_player[player_index].get()->getPlayerUnitList().size() << endl;
    if(list_player[player_index].get()->getPlayerBuildingList().size())
        cout << "error, the size of building list is not 0 but "<< list_player[player_index].get()->getPlayerBuildingList().size() << endl;
    list_dead_player.push_back(list_player[player_index]);
    list_player.erase(list_player.begin() + player_index);
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

bool State::deleteUnit(state::Unit* deleting_unit, bool keep_track) {
    bool success = false;
    //recherche du play associé à l'unité détruite
    for(int i= 0; i < (int) player_nbr;i++){
        if(deleting_unit->getPlayerId() == list_player[i].get()->getPlayerId()) {
            success = list_player[i].get()->deletePlayerUnit(deleting_unit, keep_track);
            cout << success << endl;
            break;
        }
    }
    //vérifie la suppression du pointeur dans la liste du joueur
    if(success) success = game_map.get()->deleteGameObject(deleting_unit, keep_track);
    //retourne true si la suppression * de l'unité dans la liste de map a été effectuée
    cout << success << endl;
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return success;
}


bool State::deleteBuilding(state::Building* deleting_building, bool keep_track) {
    bool success = false;
    //recherche du player associé au batiment détruit
    for(int i= 0; i < (int) player_nbr;i++){
        if(deleting_building->getPlayerId() == list_player[i].get()->getPlayerId()) {
            Player * attacked_player = list_player[i].get();
            success = attacked_player->deletePlayerBuilding(deleting_building, keep_track);
            break;
        }
    }
    if(success) {
        success = game_map.get()->deleteGameObject(deleting_building, keep_track);
           if (!success)
               return false;
    }
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

bool State::resetInRange() {
    inrange_ennemies.clear();
    return true;
}


bool State::isGameFinished(){
    if(remaining_players < 2)
        return true;
    return false;
}

bool State::incObjectCount(bool inc) {
    if(inc)
        this->object_count ++;
    else
        this->object_count --;
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

//renvoit le joueur en fonction du joueur id
shared_ptr<Player> State::getPlayer(unsigned int player_id) {
    for(shared_ptr<Player> players : list_player){
        if(players.get()->getPlayerId() == player_id)
            return players;
    }
    return nullptr;
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

vector<shared_ptr<GameObject>>& State::getInRange() {
    return this->inrange_ennemies;
}

unsigned int State::getObjectCount() {
    return this->object_count;
}

bool State::setPlayerDead(unsigned int player_id){
    for(shared_ptr<Player> killed_p : this->list_player){
        if(player_id == killed_p.get()->getPlayerId())
            killed_p.get()->setIsDead();
    }

    cout << "remaining players before change " << this->remaining_players << endl;
    this->remaining_players--;
    cout << "remaining players after change " << this->remaining_players << endl;
    return true;
}

bool State::setRemainingPlayer(int remaining_players) {
    this->remaining_players = remaining_players;
    return true;
}

bool State::setPlayerAlive(unsigned int player_id) {
    for(shared_ptr<Player> reviving_p : this->list_player){
        if(player_id == reviving_p.get()->getPlayerId())
            reviving_p.get()->setIsAlive();
    }
    this->remaining_players++;
    if(!current_player_id == player_nbr)
        this->current_player_id ++;
    return true;
}

bool State::revivePlayer(unsigned int player_id) {
    return true;
}

//met à jour l'id de joueur courant après un fin de tour
bool State::setCurrentPlayerId(bool increment) {
    if(!increment)//hacky stuff to decrement the player id when rolling back without modifying any existing code
    {
        if(this->current_player_id==0)
        {
            this->current_player_id=this->player_nbr-1;
            this->day --;
            Event event = Event(EventTypeId::UNIT_CHANGED);
            notifyObservers(event);
            return true;
        }
        this->current_player_id--;
        Event event = Event(EventTypeId::UNIT_CHANGED);
        notifyObservers(event);
        return true;
    }

    if(this->current_player_id == this->player_nbr - 1) { //le player_id commence à 0 tandis que le player_nbr commence à 1
        this->current_player_id = 0;
        this->day++;
    }

    else
        this->current_player_id ++;

    //incrémente une nouvelle fois si le joueur courant est un joueur mort
    for(shared_ptr<Player> dead_players : list_dead_player){
        if(this->current_player_id == dead_players.get()->getPlayerId()){
            if(this->current_player_id == this->player_nbr - 1) {
                this->current_player_id = 0;
                this->day++;
            }
            else
                this->current_player_id ++;
        }
    }

    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return true;
}

//met à jour le joueur courant : toujours executer après le setCurrentPlayerId
bool State::setCurrentPlayer() {
    this->current_player = getPlayer(this->current_player_id);
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return true;
}


//met à jour le nombre de "tour"
bool State::setDay(bool increment) {
    if(!increment)
    {
        if(this->current_player_id==this->player_nbr-1) this->day --;
        Event event = Event(EventTypeId::UNIT_CHANGED);
        notifyObservers(event);
        return true;
    }
    if(this->current_player_id == 0) {
        for(shared_ptr<Player> dead_players : list_dead_player){
            if(dead_players.get()->getPlayerId() == current_player_id)
                this->day ++;
        }
        this->day ++;
    }
    Event event = Event(EventTypeId::UNIT_CHANGED);
    notifyObservers(event);
    return true;
}

bool State::setPlayerNumber(unsigned int player_nbr) {
    this->player_nbr = player_nbr;
    return true;
}

bool State::setInRange(std::vector<std::shared_ptr<state::GameObject>> inrange_ennemies) {
    for (int i = 0; i < (int)inrange_ennemies.size(); i++){
        this->inrange_ennemies.push_back(inrange_ennemies[i]);
    }
    return true;
}


State::~State(){
    cout << " détruit " << this << endl;
}

std::shared_ptr<GameObject> State::getGameObject(unsigned int game_object_id) {
    return this->game_map.get()->getGameObject(game_object_id);
}

bool State::reviveGameObject(unsigned int game_object_id, unsigned int player_id, bool is_static) {
    this->getMap()->revive(game_object_id);
    for(shared_ptr<Player> players: list_player){
        if(players.get()->getPlayerId() == player_id){
            if(is_static)
                players.get()->revivePlayerBuilding(game_object_id);
            else
                players.get()->revivePlayerUnit(game_object_id);
        }
    }
}

bool State::getTurnStatus() {
    return turn_end;
}

bool State::setTurnStatus(bool end) {
    this->turn_end = end;

}