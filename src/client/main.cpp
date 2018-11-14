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
    }

    return 0;
}

bool test_randomAI() {
    // state to test

    State test_state = State(1);

    static int const terrain_int[] = {
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
    for (int i = 0; i < 256; i++) {
        test_terrain.push_back(terrain_int[i]);
    }
    Map test_map = Map(16, 16, test_terrain);

    //init map terrain
    test_state.initializeMap(test_map);

    Map *thisMap = test_state.getMap();


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
    render::DrawManager testdraw = render::DrawManager(test_state, window);

    cout << "test : new randomAI instance" << endl;
    ai::RandomAI test_randomAI = ai::RandomAI(0);



    cout << "test : updates... " << endl;
    testdraw.updateState(test_state);
    sf::sleep(delayTime);



    //init player AI
    Player test_player = Player();
    test_state.initializePlayer(test_player);

    //create the first units
    cout << "test : create unit in 6,6" << endl;
    HandleCreation test_creation = HandleCreation();
    test_creation.execute(test_state, 6, 6, 1, false);//should instanciate an unit in 10,10
    //testdraw.updateState(test_state);

    cout << "test : create unit in 0,0" << endl;
    test_creation.execute(test_state, 0, 0, 1, false);//should instanciate an unit in 0,0
    //testdraw.updateState(test_state);


    cout << "test : create unit in 3,1" << endl;
    test_creation.execute(test_state, 1, 0, 1, false);//should instanciate an unit in 3,1
    testdraw.updateState(test_state);

    test_randomAI.run(test_engine, test_player, test_state);
}


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

    Map * thisMap = testState.getMap();

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
    thisMap->addObserver(&testdraw);
    sf::sleep(delayTime);

    cout << "test : new unit in 0,1"<<endl;
    test_creation.execute(testState,0,1,1,false);//should instanciate a unit in 1,1

    cout << "test : updates... "<<endl;
    //testdraw.updateState(testState);
    sf::sleep(delayTime);


    cout << "test : cheating sequence -- start"<<endl;

    //cheating a bit to recover a unit pointer:
    vector<GameObject *> onTheTile;
    cout << "debug : a"<<endl;

    thisMap->getGameObject(0,1,&onTheTile);
    cout << "debug : b"<<endl;
    Unit * theUnit = (Unit*)onTheTile[0];
    //end of cheating part

    cout << "test : cheating sequence -- end"<<endl;


    test_movement.execute(*theUnit,testState,0,0);

    //testdraw.updateState(testState);



    // end window

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        int x_cart = 0, y_cart = 0;
        int x_iso = 0, y_iso = 0;
        int offset = 8;
        //int cc = -(y * t_map_x);
        int t_map_x = 64;
        int t_map_y = 32;
        int cc;
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

                    x_iso = 0.5*x_cart - 0.5*y_cart;
                    //y_iso =
                    std::cout << "mouse x carte: " << x_cart<< std::endl; // valeur de x en cartésien
                    std::cout << "mouse x iso: " << x_iso << std::endl; // valeur de x en cartésien
                    std::cout << "mouse y carte: " << y_cart << std::endl; // valeur de y en cartésien
                    std::cout << "mouse y iso: " << y_iso << std::endl; // valeur de y en cartésien
                    test_movement.execute(*theUnit, testState,(event.mouseButton.x)/64,(event.mouseButton.y)/32 );

                }
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

    }

    return true;
}

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
//le test ci-dessous permet de visualiser la map isométrique avec la texture du terrain.
/*
bool test_render1(){
    //size of the level
    size_t width = 16;//Map.size x
    size_t height = 16;


    //size of the window
    size_t x_window = 1024;
    size_t y_window = 1024;
    // create the window
    sf::RenderWindow window(sf::VideoMode(x_window, y_window), "Tilemap");
    sf::Vector2u t_map = sf::Vector2u(64,32);
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    if (!m_tileset.loadFromFile("res/tileset.png")) {
        return false;
    }


    // define the level with an array of tile indices
    const int level[] =
            {
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

    const int* tiles = level;
    // create the tilemap from the level definition

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    sf::Transform transform;

    int offset = width*32;

    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / t_map.x);
            int tv = tileNumber / (m_tileset.getSize().x / t_map.x);


            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // define its 4 corners
            int cc = -(j * t_map.x);
            int k = (i * t_map.x)/2 + cc/2 + t_map.x/2 + offset;
            int l = ((i + j) * t_map.y) / 2;


            quad[0].position = sf::Vector2f(k, l);
            quad[1].position = sf::Vector2f(k + t_map.x/2, l + t_map.y/2);
            quad[3].position = sf::Vector2f(k - t_map.x/2, l + t_map.y /2);
            quad[2].position = sf::Vector2f(k , l + t_map.y);

            // define its 4 texture coordinates1

            int ku = tu*t_map.x + t_map.x/2;

            quad[0].texCoords = sf::Vector2f(ku, 0);
            quad[1].texCoords = sf::Vector2f(ku + t_map.x/2, t_map.y/2);
            quad[3].texCoords = sf::Vector2f(ku - t_map.x/2, t_map.y/2);
            quad[2].texCoords = sf::Vector2f(ku , t_map.y);
        }


    sf::RenderStates r_states;
    sf::Transformable my_transformation;


    //rotates the map 45°
    //my_transformation.rotate(45);
    //set the origin of the map to the top center of the window
    //my_transformation.move(x_window/2,y_window/2);
    //scales the map to resize it
    //my_transformation.scale(1,2);
    // apply the transform
    //r_states.transform *= my_transformation.getTransform();


    // apply the tileset texture
    r_states.texture = &m_tileset;



    // run the program as long as the window is open
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

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(m_vertices,r_states);

        // end the current frame
        window.display();
    }
    return true;
}*/