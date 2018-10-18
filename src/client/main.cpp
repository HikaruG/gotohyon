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

void test();

int main(int argc,char* argv[]) 
{   
    if ( argc > 1 ){
        if ( !strcmp(argv[1],"hello") ){
            cout << "It works !" << endl;
        }
        if ( !strcmp(argv[1],"state") ){
            cout << "launching test sequence" << endl;
            test();
        }
    }

    return 0;
}

void test()
{
    Map test_map = Map(2,3,{0,0,0,0,0,0});
    cout << "map sucessfully instanciated" << endl;
}