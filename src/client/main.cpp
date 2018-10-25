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

using namespace std;
using namespace state;

bool test_state();
bool test_render1();

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
            cout << "launching render sequence" << endl;
            if (test_render1()) {
                cout << "tests successful" << endl;
            }
        }
    }

    return 0;
}

bool test_state()
{
    Map test_map = Map(2,3,{0,0,0,0,0,0});
    cout << "map sucessfully instanciated" << endl;
    Position default_pos = Position(0,0);
    cout << "trying to put a mine and an infantry in 0,0" << endl;
    if(test_map.addGameObject(1,default_pos,true, 1))
    {
        cout<<"mine is set up !"<<endl;
    }
    else
    {
        cout<<"bug !"<<endl;
        return false;
    }

    if(test_map.addGameObject(1,default_pos,false, 1))
    {
        cout<<"infantry is set up !"<<endl;
    }
    else
    {
        cout<<"bug !"<<endl;
        return false;
    }
    vector<GameObject*> *lgo = new vector<GameObject*>;
    test_map.getGameObject(0,0,lgo);
    cout<<"number of item in 0,0:"<<lgo->size()<<endl;
    cout<<"damaging everyone ! "<<endl;
    for(int i =0; i< lgo->size();i++) {
        (*lgo)[i]->takeDamage(12);
        int health;
        (*lgo)[i]->getHealth(&health);
        cout << "lgo health:" << health << endl;
    }
    cout<<"moving everyone !? "<<endl;
    Position new_position = Position(1,0);
    for(int i =0; i< lgo->size();i++) {
        if(test_map.moveGameObject((*lgo)[i]->getGame_object_id(),new_position))
        {
            cout<<"successfully moved "<<(*lgo)[i]->getGame_object_id()<<endl;
        }
        else{
            cout<<"actually i can't move "<<(*lgo)[i]->getGame_object_id()<<endl;
            Property *prop = new Property();
            (*lgo)[i]->getProperty(prop);
            cout << "it's a "<<prop->getStringType()<<" that i can't move !"<<endl;
        }
    }

    cout << "dope" << endl;
    return true;
}

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

    if (!m_tileset.loadFromFile("../src/tileset.png")) {
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
/*
    const int level[] =
            {
                2,2,
                2,2
            };
*/
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
            cout <<"new tile : "<<tileNumber<<endl;

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / t_map.x);
            int tv = tileNumber / (m_tileset.getSize().x / t_map.x);

            cout << "tu: "<<tu<<" tv: "<<tv<<endl;

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            cout << "i: "<<i<<" j: "<<j<<endl;

            // define its 4 corners
            int cc = -(j * t_map.x);
            cout << "cc : "<<cc<<endl;
            int k = (i * t_map.x)/2 + cc/2 + t_map.x/2 + offset;
            int l = ((i + j) * t_map.y) / 2;

            cout << "k: "<<k<<" l: "<<l<<endl;

            quad[0].position = sf::Vector2f(k, l);
            quad[1].position = sf::Vector2f(k + t_map.x/2, l + t_map.y/2);
            quad[3].position = sf::Vector2f(k - t_map.x/2, l + t_map.y /2);
            quad[2].position = sf::Vector2f(k , l + t_map.y);

            // define its 4 texture coordinates1

            int ku = tu*t_map.x + t_map.x/2;

            cout << "ku: "<<ku << endl;

            quad[0].texCoords = sf::Vector2f(ku, 0);
            quad[1].texCoords = sf::Vector2f(ku + t_map.x/2, t_map.y/2);
            quad[3].texCoords = sf::Vector2f(ku - t_map.x/2, t_map.y/2);
            quad[2].texCoords = sf::Vector2f(ku , t_map.y);
            cout << endl;
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
}