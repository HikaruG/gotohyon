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