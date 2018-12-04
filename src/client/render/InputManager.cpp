//
// Created by ben on 04/12/18.
//

#include "InputManager.h"
#include <iostream>

using namespace render;

InputManager::InputManager(std::shared_ptr<sf::RenderWindow> window)
{
 std::cout<<"input manager is up and foolowing event on window "<<&window<<std::endl;
 this->window=window;
}

bool InputManager::getClickLocation ()
{
    sf::Event event;
    int x_cart = 0, y_cart = 0;
    int x_iso = 0, y_iso = 0;
    float t_map_x = 64;
    float t_map_y = 32;

    window.get()->pollEvent(event);

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            std::cout << "the right button was pressed" << std::endl;
            x_cart = (event.mouseButton.x)/t_map_x;
            y_cart = (event.mouseButton.y)/t_map_y;

            x_iso = y_cart - x_cart;
            y_iso = y_cart + x_cart - 8;
            std::cout << "mouse x carte: " << x_cart<< std::endl; // valeur de x en cartésien
            std::cout << "mouse x iso: " << x_iso << std::endl; // valeur de x en cartésien
            std::cout << "mouse y carte: " << y_cart << std::endl; // valeur de y en cartésien
            std::cout << "mouse y iso: " << y_iso << std::endl; // valeur de y en cartésien

        }
    }
    // "close requested" event: we close the window

    return true;
}

bool InputManager::getButtonPressed ()
{

    return true;
}