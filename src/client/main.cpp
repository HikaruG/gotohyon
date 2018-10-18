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

bool test();

int main(int argc,char* argv[]) 
{   
    if ( argc > 1 ){
        if ( !strcmp(argv[1],"hello") ){
            cout << "It works !" << endl;
        }
        if ( !strcmp(argv[1],"state") ){
            cout << "launching test sequence" << endl;
            if(test())
            {
                cout<<"tests successful"<< endl;
            }
        }
    }

    return 0;
}

bool test()
{
    Map test_map = Map(2,3,{0,0,0,0,0,0});
    cout << "map sucessfully instanciated" << endl;
    Position default_pos = Position(0,0);
    cout << "trying to put a mine in 0,0" << endl;
    if(test_map.addGameObject(1,default_pos,true, 1))
    {
        cout<<"it worked !"<<endl;
    }
    else
    {
        cout<<"bug !"<<endl;
        return false;
    }
    return true;
}