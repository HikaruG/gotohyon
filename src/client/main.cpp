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
bool test_render();

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
            if (test_render()) {
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

bool test_render(){
    size_t width = 16;
    size_t height = 8;

    // create the window
    sf::RenderWindow window(sf::VideoMode(512, 256), "Tilemap");
    sf::Vector2u t_map = sf::Vector2u(32,32);
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    if (!m_tileset.loadFromFile("/media/sf_dossier_commun/tileset.png")) {
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
            };


    const int* tiles = level;
    // create the tilemap from the level definition

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    sf::Transform transform;


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
            quad[0].position = sf::Vector2f(i * t_map.x, (j * t_map.y)/2);
            quad[1].position = sf::Vector2f((i + 1) * t_map.x, (j * t_map.y)/2);
            quad[2].position = sf::Vector2f((i + 1) * t_map.x, ((j + 1) * t_map.y)/2);
            quad[3].position = sf::Vector2f(i * t_map.x, ((j + 1) * t_map.y)/2);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * t_map.x, (tv * t_map.y)/2);
            quad[1].texCoords = sf::Vector2f((tu + 1) * t_map.x, (tv * t_map.y)/2);
            quad[2].texCoords = sf::Vector2f((tu + 1) * t_map.x, ((tv + 1) * t_map.y)/2);
            quad[3].texCoords = sf::Vector2f(tu * t_map.x, ((tv + 1) * t_map.y)/2);
        }
        //t_map.rotate(45);


    sf::RenderStates r_states;
    sf::Transformable my_transformation;


    // apply the transform
    //my_transformation.rotate(45);
    r_states.transform *= my_transformation.getTransform();


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

}