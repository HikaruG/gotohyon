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

/*
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
                cout << "render testing" << endl;
            }
        }
        if( !strcmp(argv[1],"engine") ) {
            cout << "engine test sequence activated . . ." << endl;
            if (test_engine()) {
                cout << "render testing" << endl;
            }
        }
        if( !strcmp(argv[1],"random_ai") ) {
            cout << "test random ai sequence activated . . ." << endl;
            if (test_randomAI()) {
                cout << "test random_ai successful" << endl;
            }
        }
    }

    return 0;
}
*/

int main(int argc,char* argv[]) {
    if (argc > 1) {
        if( !strcmp(argv[1],"random_ai") ) {
            cout << "test random ai sequence activated . . ." << endl;
            if (test_randomAI()) {
                cout << "test random_ai successful" << endl;
            }
        }
    }
}

bool test_randomAI() {
    // state to test

    shared_ptr<State> test_state (new State(1));

    static int const terrain_int[] = {
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
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

    sf::Time delayTime = sf::milliseconds(1000);
    // create the window
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)),
                            "test engine");


    cout << "test : new engine instance" << endl;
    Engine test_engine = Engine();


    cout << "test : new drawmanager instance" << endl;
    render::DrawManager testdraw = render::DrawManager(move(test_state), window);

    cout << "test : new randomAI instance" << endl;
    ai::RandomAI test_randomAI = ai::RandomAI(0);



    cout << "test : updates... " << endl;
    testdraw.updateState(move(test_state));
    sf::sleep(delayTime);



    //init player AI
    test_state->initializePlayer();


    State * ptr_test_state = test_state.get();
    //create the first units
    cout << "test : create unit in 6,6" << endl;
    HandleCreation test_creation = HandleCreation();
    test_creation.execute(* test_state.get(), 6, 6, 1, false);//should instanciate an unit in 6,6
    //testdraw.updateState(test_state);

    cout << "test : create unit in 0,0" << endl;
    test_creation.execute(* test_state.get(), 0, 0, 1, false);//should instanciate an unit in 0,0
    //testdraw.updateState(test_state);


    cout << "test : create unit in 3,1" << endl;
    test_creation.execute(* test_state.get(), 1, 0, 1, false);//should instanciate an unit in 3,1
    testdraw.updateState(move(test_state));

    cout << "test : create building in 0,0"<<endl;
    test_creation.execute(* test_state.get(),0,0,1,true);//should instanciate a building in 0,0
    testdraw.updateState(move(test_state));

    Player * current_player = test_state.get()->getCurrentPlayer(0).get();

    for(int i =0; i<100;i++) {
        test_randomAI.run(test_engine, * current_player, * test_state.get());
        testdraw.updateState(move(test_state));
        sf::sleep(delayTime);
    }

    return true;
}

/*
bool test_engine()
{
    // state to test
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
    vector<int> test_terrain;//to do mettre la taille
    for (int i = 0; i < 256;i++)
    {
        test_terrain.push_back(terrain_int[i]);
    }
    Map test_map = Map(16,16,test_terrain);

    testState.initializeMap(test_map);

    Map thisMap = testState.getMap();

    //init window

    //init windows
    size_t x_window = 1024;
    size_t y_window = 1024;

    sf::Time delayTime = sf::milliseconds(1000);
    // create the window
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)), "test engine");

    // tests starts here
    cout << "test : new engine instance"<<endl;
    Engine test_engine = Engine();

    cout << "test : new handlers instance"<<endl;
    HandleCreation test_creation = HandleCreation();
    HandleMovement test_movement = HandleMovement();

    cout << "test : create building in 0,0"<<endl;
    test_creation.execute(testState,0,0,1,true);//should instanciate a building in 0,0

    cout << "test : new drawmanager instance"<<endl;
    render::DrawManager testdraw = render::DrawManager(testState,window);
    testState.addObserver(&testdraw);
    thisMap.addObserver(&testdraw);
    sf::sleep(delayTime);

    cout << "test : new unit in 0,1"<<endl;
    test_creation.execute(testState,0,1,1,false);//should instanciate a unit in 1,1

    cout << "test : updates... "<<endl;
    //testdraw.updateState(testState);
    sf::sleep(delayTime);


    cout << "test : cheating sequence -- start"<<endl;

    //cheating a bit to recover a unit pointer:
    ///*
    vector<GameObject *> onTheTile;
    cout << "debug : a"<<endl;

    thisMap->getGameObject(0,1,&onTheTile);
    cout << "debug : b"<<endl;
    Unit * theUnit = (Unit*)onTheTile[0];
    //*/
     //end of cheating part
/*

    cout << "test : cheating sequence -- end"<<endl;


    //test_movement.execute(*theUnit,testState,0,0);

    //testdraw.updateState(testState);



    // end window

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        float x_cart = 0, y_cart = 0;
        float x_iso = 0, y_iso = 0;
        //int cc = -(y * t_map_x);
        float t_map_x = 64;
        float t_map_y = 32;
        //int k = (x * t_map_x)/2 + cc/2 + t_map_x/2 + offset;
        //int l = ((x + y) * t_map_y) / 2;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "the right button was pressed" << std::endl;
                    x_cart = (event.mouseButton.x)/t_map_x;
                    y_cart = (event.mouseButton.y)/t_map_y;

                    x_iso = x_cart - 8 + y_cart;
                    y_iso = y_cart - x_cart + 8;
                    std::cout << "mouse x carte: " << x_cart<< std::endl; // valeur de x en cartésien
                    std::cout << "mouse x iso: " << x_iso << std::endl; // valeur de x en cartésien
                    std::cout << "mouse y carte: " << y_cart << std::endl; // valeur de y en cartésien
                    std::cout << "mouse y iso: " << y_iso << std::endl; // valeur de y en cartésien

                }
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

    }

    return true;
}
 */
/*
bool test_state()
{
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


    return true;
}

 */

/*
bool test_render(){

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
    vector<int> test_terrain;//to do mettre la taille
    for (unsigned int i = 0; i < 256;i++)
    {
        test_terrain.push_back(terrain_int[i]);
    }
    Map test_map = Map(16,16,test_terrain);

    testState.initializeMap(test_map);

    Map * thisMap = testState.getMap();

    //adding some stuff to the map
    Position default_pos = Position(0,0);

    thisMap->addGameObject(1,default_pos,true,2);
    default_pos.setPosition(1,2);
    thisMap->addGameObject(1,default_pos,true, 2);
    default_pos.setPosition(1,5);
    thisMap->addGameObject(1,default_pos,true, 2);
    default_pos.setPosition(6,2);
    thisMap->addGameObject(1,default_pos,false, 2);
    default_pos.setPosition(14,7);
    thisMap->addGameObject(1,default_pos,false, 2);


    //init windows
    size_t x_window = 1024;
    size_t y_window = 1024;
    // create the window
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(x_window), static_cast<unsigned int>(y_window)), "Tilemap");

    render::DrawManager testdraw = render::DrawManager(testState,window);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

    }
    return true;

}

 */