#include <iostream>
#include <stdio.h>
#include <string.h>

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"
#include "render.h"
#include "engine.h"
#include "ai.h"

using namespace std;
using namespace state;
using namespace engine;

bool test_state();
bool test_render();
bool test_engine();
bool test_randomAI();
bool test_heuristicAI();
bool test_deepAI();
bool test_input();


int main(int argc,char* argv[])
{
    if ( argc > 1 ){

        if ( !strcmp(argv[1],"hello") ){
            cout << "It works !" << endl;
        }
        if ( !strcmp(argv[1],"state") ){
            cout << "launching test sequence" << endl;
            if(test_state())
            {
                cout<<"tests successful"<< endl;
            }
        }

        if( !strcmp(argv[1],"render") ) {
            cout << "render test sequence  activated . . ." << endl;
            if (test_render()) {
                cout << "test render successful" << endl;
            }
        }

        if( !strcmp(argv[1],"engine") ) {
            cout << "engine test sequence activated . . ." << endl;
            if (test_engine()) {
                cout << "test render successful" << endl;
            }
        }
        if( !strcmp(argv[1],"random_ai") ) {
            cout << "test random ai sequence activated . . ." << endl;
            if (test_randomAI()) {
                cout << "test random_ai successful" << endl;
            }
        }
        if( !strcmp(argv[1],"heuristic_ai") ) {
            cout << "test heuristic ai sequence activated . . ." << endl;
            if (test_heuristicAI()) {
                cout << "test heuristic_ai successful" << endl;
            }
        }
        if( !strcmp(argv[1],"deep_ai") ) {
            cout << "test deep_ai activated . . ." << endl;
            if (test_deepAI()) {
                cout << "test deep_ai successful" << endl;
            }
        }
        if( !strcmp(argv[1],"input") ) {
            cout << "test input activated . . ." << endl;
            if (test_input()) {
                cout << "test input successful" << endl;
            }
        }
    }

    return 0;
}



bool test_input(){
    shared_ptr<State> test_state (new State(2,2));

    static int const terrain_int [] = {
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 0, 3, 3, 2, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 0, 1, 0, 3, 0, 3, 3, 0, 0, 1, 1, 1, 1, 2, 0,
            2, 0, 1, 0, 3, 0, 2, 3, 3, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 2, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1,
            2, 0, 1, 0, 3, 0, 3, 3, 3, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 0, 2, 3, 0, 0, 1, 1, 1, 1, 2, 0,
            0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    vector<int> test_terrain;//to do mettre la taille
    for (int i = 0; i < 256; i++) {
        test_terrain.push_back(terrain_int[i]);
    }
    //init map terrain
    test_state->initializeMap(16, 16, test_terrain);

    //Map * thisMap = test_state.getMap().get();

    //init windows
    size_t x_window = 1024;
    size_t y_window = 512;

    //sf::Time delayTime = sf::milliseconds(1000);
    // create the window
    shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)),
                                                              "test engine",sf::Style::Close));



    cout << "test : new engine instance" << endl;
    Engine test_engine = Engine();


    cout << "test : new drawmanager instance" << endl;
    render::DrawManager testdraw = render::DrawManager(test_state, window);
    test_state.get()->addObserver(&testdraw);
    test_state.get()->getMap().get()->addObserver(&testdraw);

    cout << "test : new randomAI instance" << endl;
    ai::RandomAI test_randomAI = ai::RandomAI(0);



    cout << "test : updates... " << endl;
    testdraw.forceRefresh(test_state);

    while(!test_state.get()->isGameFinished())
    {

    }
    return true;

}

bool test_deepAI(){

    shared_ptr<state::State> test_state (new state::State(2,2));
    cout << "test : new state instance" << endl;
    Engine test_engine = Engine();
    cout << "test : new engine instance" << endl;
    HandleStartGame new_game = HandleStartGame(2);
    new_game.execute(*test_state.get(),test_engine);
    test_engine.execute(* test_state.get());
    cout << "test : 1 player 2 npcs gamemode created " << endl;

    //init windows
    size_t x_window = 1024;
    size_t y_window = 512;

    sf::Time delayTime = sf::milliseconds(1000);
    // create the window
    shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)),
                                                              "test engine",sf::Style::Close));

    render::DrawManager testdraw = render::DrawManager(test_state, window);
    cout << "test : new drawmanager instance" << endl;
    test_state.get()->addObserver(&testdraw);
    test_state.get()->getMap().get()->addObserver(&testdraw);


    cout << "remaining players " << test_state.get()->getRemainingPlayers() << endl;
    cout << "test : new DeepAI instance" << endl;
    ai::DeepAI test_deepAI = ai::DeepAI(0);


    //cout << "test : updates... " << endl;
    testdraw.forceRefresh(test_state);

    while(window.get()->isOpen()) {
        test_engine.execute(* test_state.get());
        if(test_state.get()->getCurrentPlayer().get()->getIsNpc()) {
            test_deepAI.run(test_engine, *test_state.get());
            testdraw.forceRefresh(test_state);
            sf::sleep(delayTime);
        } else
            testdraw.user_interact.userTurn(test_engine,*test_state.get());
    }

    return true;
}


bool test_heuristicAI(){


    shared_ptr<state::State> test_state (new state::State(2,2));
    cout << "test : new state instance" << endl;
    Engine test_engine = Engine();
    cout << "test : new engine instance" << endl;
    HandleStartGame new_game = HandleStartGame(2);
    new_game.execute(*test_state.get(),test_engine);
    test_engine.execute(* test_state.get());
    cout << "test : 2 npc gamemode created " << endl;

    //init windows
    size_t x_window = 1024;
    size_t y_window = 512;

    sf::Time delayTime = sf::milliseconds(1000);
    // create the window
    shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)),
                                                              "test engine",sf::Style::Close));

    render::DrawManager testdraw = render::DrawManager(test_state, window);
    cout << "test : new drawmanager instance" << endl;
    test_state.get()->addObserver(&testdraw);
    test_state.get()->getMap().get()->addObserver(&testdraw);


    cout << "test : new HeuristicAI instance" << endl;
    ai::HeuristicAI test_heuristicAI = ai::HeuristicAI(0);


    //cout << "test : updates... " << endl;
    testdraw.forceRefresh(test_state);

    while(!test_state.get()->isGameFinished()) {
        test_engine.execute(* test_state.get());
        if(test_state.get()->getCurrentPlayer().get()->getIsNpc()) {
            test_heuristicAI.run(test_engine, *test_state.get());
            testdraw.forceRefresh(test_state);
            sf::sleep(delayTime);
        }

    }

    return true;
}



bool test_randomAI() {

    shared_ptr<state::State> test_state (new state::State(2,1));
    cout << "test : new state instance" << endl;
    Engine test_engine = Engine();
    cout << "test : new engine instance" << endl;
    HandleStartGame new_game = HandleStartGame(2);
    new_game.execute(*test_state.get(),test_engine);
    test_engine.execute(* test_state.get());
    cout << "test : 1  player vs 1 npc gamemode created " << endl;


    //init windows
    size_t x_window = 1024;
    size_t y_window = 512;

    sf::Time delayTime = sf::milliseconds(1000);
    // create the window
    shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)),
                                                              "test engine",sf::Style::Close));

    render::DrawManager testdraw = render::DrawManager(test_state, window);
    cout << "test : new drawmanager instance" << endl;
    test_state.get()->addObserver(&testdraw);
    test_state.get()->getMap().get()->addObserver(&testdraw);


    cout << "test : new randomAI instance" << endl;
    ai::RandomAI test_randomAI = ai::RandomAI(0);

    testdraw.forceRefresh(test_state);

    while(!test_state.get()->isGameFinished()) {
        test_engine.execute(* test_state.get());
        if(test_state.get()->getCurrentPlayer().get()->getIsNpc()) {
            test_randomAI.run(test_engine, *test_state.get());
            testdraw.forceRefresh(test_state);
            sf::sleep(delayTime);
        }
        else{
            testdraw.user_interact.userTurn(test_engine,*test_state.get());
        }

    }

    return true;
}


bool test_engine()
{/*
    shared_ptr<State> test_state (new State(2,2));

    static int const terrain_int [] = {
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 0, 3, 3, 2, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 0, 1, 0, 3, 0, 3, 3, 0, 0, 1, 1, 1, 1, 2, 0,
            2, 0, 1, 0, 3, 0, 2, 3, 3, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 2, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1,
            2, 0, 1, 0, 3, 0, 3, 3, 3, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 0, 2, 3, 0, 0, 1, 1, 1, 1, 2, 0,
            0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    vector<int> test_terrain;//to do mettre la taille
    for (int i = 0; i < 256; i++) {
        test_terrain.push_back(terrain_int[i]);
    }
    //init map terrain
    test_state->initializeMap(16, 16, test_terrain);

    //Map * thisMap = test_state.getMap().get();

    //init windows
    size_t x_window = 1024;
    size_t y_window = 1024;

//    sf::Time delayTime = sf::milliseconds(1000);
    // create the window
    shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)),
                            "test engine"));



    cout << "test : new engine instance" << endl;
    Engine test_engine = Engine();


    cout << "test : new drawmanager instance" << endl;
    render::DrawManager testdraw = render::DrawManager(test_state, window);
    test_state.get()->addObserver(&testdraw);
    test_state.get()->getMap().get()->addObserver(&testdraw);



    // tests starts here

    cout << "test : updates... " << endl;
    testdraw.forceRefresh(test_state);



    HandleCreation test_creation = HandleCreation();
    HandleTurn test_turn = HandleTurn();
    HandleMovement test_movement = HandleMovement();



    cout << "test : create units and building for the 1st npc" << endl;
    //create the first units for the 1st npc
    cout << "test : create unit in 7,2" << endl;
    test_creation.execute(* test_state.get(), 7, 2, 1, false);//should instanciate an unit in 6,6
    //testdraw.forceRefresh(test_state);

    cout << "test : create unit in 8,2" << endl;
    test_creation.execute(* test_state.get(), 8, 2, 1, false);//should instanciate an unit in 0,0
    //testdraw.forceRefresh(test_state);


    cout << "test : create unit in 6,2" << endl;
    test_creation.execute(* test_state.get(), 6, 2, 1, false);//should instanciate an unit in 3,1
    testdraw.forceRefresh(test_state);

    cout << "test : create building in 7,2"<<endl;
    test_creation.execute(* test_state.get(),7,2,3,true);//should instanciate a building in 0,0
    testdraw.forceRefresh(test_state);


    //end 1st npc's first turn
    cout << "npc1 : end 1st turn"<<endl;
    test_turn.execute(*test_state.get());

    //create the first units for the 2nd npc


    cout << "test : create units and building for the 2nd npc" << endl;
    cout << "test : create unit in 7,13" << endl;
    test_creation.execute(* test_state.get(), 7, 13, 1, false);//should instanciate an unit in 6,6
    //testdraw.forceRefresh(test_state);

    cout << "test : create unit in 8,13" << endl;
    test_creation.execute(* test_state.get(), 8, 13, 1, false);//should instanciate an unit in 0,0
    //testdraw.forceRefresh(test_state);


    cout << "test : create unit in 6,13" << endl;
    test_creation.execute(* test_state.get(), 6, 13, 1, false);//should instanciate an unit in 3,1
    testdraw.forceRefresh(test_state);

    cout << "test : create building in 7,13"<<endl;
    test_creation.execute(* test_state.get(),7,13,3,true);//should instanciate a building in 0,0
    testdraw.forceRefresh(test_state);

    //end 2nd npc's first turn
    cout << "npc2 : end 1st turn"<<endl;
    test_turn.execute(*test_state.get());

    test_movement.execute( * test_state.get());




    // end window

    while (window.get()->isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.get()->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.get()->close();
        }

    }
*/
    return true;
}


bool test_state()
{
    /*
    State testState = State(1);
    static int const terrain_int [] = {
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
            2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
            2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
            0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    vector<int> test_terrain;
    for (int i = 0; i < 256;i++)
    {
        test_terrain.push_back(terrain_int[i]);
    }
    Map test_map = Map(16,16,test_terrain);

    testState.initializeMap(test_map);

    Map * thisMap = testState.getMap();
    cout<<"this map : "<<&thisMap<<endl;

    cout << "map sucessfully instanciated" << endl;

    Position default_pos = Position(0,0);

    cout << "trying to put a mine and an infantry in 0,0" << endl;

    if(thisMap->addGameObject(1,default_pos,true, 1))
    {
        cout<<"mine is set up !"<<endl;
    }
    else
    {
        cout<<"bug !"<<endl;
        return false;
    }

    if(thisMap->addGameObject(1,default_pos,false, 1))
    {
        cout<<"infantry is set up !"<<endl;
    }
    else
    {
        cout<<"bug !"<<endl;
        return false;
    }

    vector<GameObject*> *lgo = new vector<GameObject*>;
    thisMap->getGameObject(0,0,lgo);
    cout<<"number of item in 0,0:"<<lgo->size()<<endl;
    cout<<"damaging everyone ! "<<endl;
    for(unsigned int i =0; i< lgo->size();i++) {
        (*lgo)[i]->getProperty()->takeDamage(12);
        int health = (*lgo)[i]->getProperty()->getHealth();
        cout << "lgo health:" << health << endl;
    }
    cout<<"moving everyone !? "<<endl;
    Position new_position = Position(1,0);
    for(unsigned int i =0; i< lgo->size();i++) {
        if(thisMap->moveGameObject((*lgo)[i]->getGame_object_id(),new_position))
        {
            cout<<"successfully moved "<<(*lgo)[i]->getGame_object_id()<<endl;
        }
        else{
            cout<<"actually i can't move "<<(*lgo)[i]->getGame_object_id()<<endl;
        }
    }


     */
    return true;
}




bool test_render(){
    shared_ptr<State> test_state (new State(2,2));

    static int const terrain_int [] = {
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 0, 3, 3, 2, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 0, 1, 0, 3, 0, 3, 3, 0, 0, 1, 1, 1, 1, 2, 0,
            2, 0, 1, 0, 3, 0, 2, 3, 3, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 2, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1,
            2, 0, 1, 0, 3, 0, 3, 3, 3, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 3, 0, 2, 3, 0, 0, 1, 1, 1, 1, 2, 0,
            0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
            0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    vector<int> test_terrain;//to do mettre la taille
    for (int i = 0; i < 256; i++) {
        test_terrain.push_back(terrain_int[i]);
    }
    //init map terrain
    test_state->initializeMap(16, 16, test_terrain);

    //Map * thisMap = test_state.getMap().get();

    //init windows
    size_t x_window = 1024;
    size_t y_window = 1024;

//    sf::Time delayTime = sf::milliseconds(1000);
    // create the window
    shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)),
                                                              "test engine"));




    cout << "test : new drawmanager instance" << endl;
    render::DrawManager testdraw = render::DrawManager(test_state, window);
    test_state.get()->addObserver(&testdraw);
    test_state.get()->getMap().get()->addObserver(&testdraw);



    // tests starts here

    cout << "test : updates... " << endl;
    testdraw.forceRefresh(test_state);



    //adding some stuff to the map
    Position default_pos = Position(0,0);
    Position default_pos2 = Position(1,2);
    Position default_pos3 = Position(0,5);

    Property farmer = Property("unit_farmer",10,10,10,false,false,1);
    Property infantry = Property("unit_infantry",10,10,10,false,false,1);
    Property archer = Property("unit_archer",10,10,10,false,false,3);

    shared_ptr<Unit> test_unit1 (new Unit(1,0,0,default_pos,infantry,state::infantry));
    shared_ptr<Unit> test_unit2 (new Unit(1,0,0,default_pos2,infantry,state::infantry));
    shared_ptr<Unit> test_unit3 (new Unit(1,0,0,default_pos3,infantry,state::infantry));

    test_state.get()->getMap()->addGameObject(test_unit1);
    test_state.get()->getMap()->addGameObject(test_unit2);
    test_state.get()->getMap()->addGameObject(test_unit3);


    while (window.get()->isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.get()->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.get()->close();
        }

    }
    return true;

}
