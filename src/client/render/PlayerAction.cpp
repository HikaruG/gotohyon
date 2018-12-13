#include "PlayerAction.h"
#include <iostream>
#include "engine.h"

using namespace render;

PlayerAction::PlayerAction (std::shared_ptr<sf::RenderWindow> window){
    this->window = window;
}

void PlayerAction::userTurn (engine::Engine& engine, state::State& state){

    std::cout<<"new player started a turn"<<std::endl;

    std::shared_ptr<engine::HandleGrowth> commande_growth (new engine::HandleGrowth());
    //rajout des ressources
    engine.addCommands(commande_growth);

    bool endturn = false;

    int x_cart = 0, y_cart = 0;
    int x_iso = 0, y_iso = 0;
    float t_map_x = 64;
    float t_map_y = 32;

    while(!endturn) {
        sf::Event event;
        while (window.get()->pollEvent(event)) {
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
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Space)
                {
                    endturn = true;
                }
            }
        }
    }
    std::shared_ptr<engine::HandleTurn> commande_turn (new engine::HandleTurn());

    engine.addCommands(commande_turn);
}