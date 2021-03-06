#include "PlayerAction.h"
#include <iostream>
#include <cmath>
#include "engine.h"
#include "DrawElement.h"
#include "OnMapLayer.h"
#include "state.h"

using namespace render;

PlayerAction::PlayerAction (std::shared_ptr<sf::RenderWindow> window){
    this->window = window;
}

void PlayerAction::userTurn (engine::Engine& engine, state::State& state){

    unsigned int food, gold;
    std::cout<<"new player started a turn"<<std::endl;//aucune action ne doit etre fait sur le state sans passer par le moteur

    std::shared_ptr<engine::HandleGrowth> commande_growth (new engine::HandleGrowth(food, gold));
    std::shared_ptr<engine::HandleSaveGame> command_save (new engine::HandleSaveGame());
    //std::shared_ptr<engine::HandleMovement> move (new engine::HandleMovement());
    //std::shared_ptr<engine::HandleCreation> create (new engine::HandleCreation());
    //rajout des ressources
    engine.addCommands(commande_growth);
    state::Unit * selected_unit;
    bool endturn = false;

    int x_on_map = 0, y_on_map = 0;
    int x_on_map_old = -1, y_on_map_old = -1;
    int* ptr_x = &x_on_map;
    int* ptr_y = &y_on_map;


    std::shared_ptr<state::Map> current_map = state.getMap();

    int x_move = 0;
    int y_move = 0;
    std::shared_ptr<state::GameObject> objSel;

    while(!endturn) {
        sf::Event event;
        sf::Clock clock; // starts the clock


        std::vector<std::shared_ptr<state::GameObject>> objs;
        while (window.get()->pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Right)
                {
                    std::cout<<"deselect object"<<std::endl;
                    x_on_map_old = -1;
                    y_on_map_old = -1;
                    objSel= nullptr;
                }
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    _getClickLocation(event,ptr_x,ptr_y);


                    //this part will be also in engine later
                    if(x_on_map < 16 && x_on_map>=0 && y_on_map < 16 && y_on_map>=0)
                    {
                         objs = current_map.get()->getGameObject((unsigned int)x_on_map,(unsigned int)y_on_map);
                    }

                    std::cout <<" mouse x iso: " << x_on_map << std::endl; // valeur de x en cartésien
                    std::cout <<" mouse y iso: " << y_on_map << std::endl; // valeur de y en cartésien

                    for(unsigned int i = 0; i < objs.size();i++)//form 0 -> nothing to 2 -> unit + building
                    {
                        std::cout<<"on this location there is "<<objs.at(i).get()->getProperty()->getStringType()<<std::endl;
                    }

                    //if new loc empty + obj not static sel: new movement

                    if(objSel.get()) {
                        if(objSel.get()->getProperty()->getAvailability()){
                            if (objSel.get() && !objSel.get()->getProperty()->isStatic() && objs.size() == 0) {
                                if (state.getCurrentPlayerId() == objSel.get()->getPlayerId()) {
                                    std::cout << "Moving" << std::endl;
                                    std::shared_ptr<engine::HandleMovement> move(
                                            new engine::HandleMovement(x_on_map, y_on_map, (state::Unit *) objSel.get()));
                                    engine.addCommands(move);
                                    engine.execute(state);
                                } else {
                                    std::cout << "not your units ! " << std::endl;
                                }
                                objSel = nullptr;
                            }

                            //attack

                            if (objSel.get() && objs.size() > 0 && !objSel.get()->getProperty()->isStatic()) {
                                if (state.getCurrentPlayerId() == objSel.get()->getPlayerId() &&
                                    objs.at(0).get()->getPlayerId() != state.getCurrentPlayerId()) {
                                    std::shared_ptr<engine::HandleDamage> damage(
                                            new engine::HandleDamage((state::Unit *) objSel.get(), objs.at(0).get()));
                                    engine.addCommands(damage);
                                    engine.execute(state);
                                } else {
                                    std::cout << "not your unit or not an enemy unit" << std::endl;
                                }
                                //objSel.get()->getProperty()->setAvailability(false);
                                objSel = nullptr;
                            }
                        }
                    }

                    if (objs.size() > 1 && x_on_map == x_on_map_old && y_on_map == y_on_map_old) {
                        objSel = objs[0].get()->getProperty()->isStatic() ? objs[0] : objs[1];
                    } else if (objs.size() > 1) {
                        objSel = objs[0].get()->getProperty()->isStatic() ? objs[1] : objs[0];

                    } else if (objs.size() == 1) {
                        objSel = objs[0];
                    }
                    if (objSel) {
                        std::cout << "selected obj : " << objSel->getProperty()->getStringType() << std::endl;
                    }

                    x_on_map_old = x_on_map;
                    y_on_map_old = y_on_map;
                }

            }
            if(event.type == sf::Event::MouseWheelScrolled)
            {
                float zoom_max = 16;
                float zoom_min = -25;
                int zoom_level_temp = OnMapLayer::zoom_level;
                if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    zoom_level_temp += event.mouseWheelScroll.delta;
                    if(zoom_level_temp < zoom_max && zoom_level_temp >= zoom_min)
                    {
                        OnMapLayer::zoom_level = zoom_level_temp;
                        std::cout << "new zoom " << OnMapLayer::zoom_level << std::endl;
                        state::Event game_event = state::Event(state::EventTypeId::TERRAIN_CHANGED);
                        notifyObservers(game_event);
                    }
                }

            }
            if(event.type == sf::Event::KeyPressed)
            {

                //farmer create stuff
                if (objSel.get() && objSel.get()->getProperty()->getStringType() == "farmer")
                {
                    std::cout<<"Creation Mode : \nF: new Farm\nT: new town\nB: new Barrack\nM: new mine\nR: Turret\n"<<std::endl;
                    if(event.key.code == sf::Keyboard::F)
                    {
                        unsigned int x;
                        unsigned int y;
                        state::Position newPos;
                        newPos = objSel.get()->getPosition();
                        x = newPos.getX();
                        y = newPos.getY();
                        std::shared_ptr<engine::HandleCreation> creat (new engine::HandleCreation(x,y,state::BuildingType::farm,true));
                        engine.addCommands(creat);
                        engine.execute(state);
                        objSel = nullptr;
                    }
                    else if(event.key.code == sf::Keyboard::T)
                    {
                        unsigned int x;
                        unsigned int y;
                        state::Position newPos;
                        newPos = objSel.get()->getPosition();
                        x = newPos.getX();
                        y = newPos.getY();
                        std::shared_ptr<engine::HandleCreation> creat (new engine::HandleCreation(x,y,state::BuildingType::town,true));
                        engine.addCommands(creat);
                        engine.execute(state);
                        objSel = nullptr;
                    }
                    else if(event.key.code == sf::Keyboard::B)
                    {
                        unsigned int x;
                        unsigned int y;
                        state::Position newPos;
                        newPos = objSel.get()->getPosition();
                        x = newPos.getX();
                        y = newPos.getY();
                        std::shared_ptr<engine::HandleCreation> creat (new engine::HandleCreation(x,y,state::BuildingType::barrack,true));
                        engine.addCommands(creat);
                        engine.execute(state);
                        objSel = nullptr;
                    }
                    else if(event.key.code == sf::Keyboard::M)
                    {
                        unsigned int x;
                        unsigned int y;
                        state::Position newPos;
                        newPos = objSel.get()->getPosition();
                        x = newPos.getX();
                        y = newPos.getY();
                        std::shared_ptr<engine::HandleCreation> creat (new engine::HandleCreation(x,y,state::BuildingType::mine,true));
                        engine.addCommands(creat);
                        engine.execute(state);
                        objSel = nullptr;
                    }
                    else if(event.key.code == sf::Keyboard::R)
                    {
                        unsigned int x;
                        unsigned int y;
                        state::Position newPos;
                        newPos = objSel.get()->getPosition();
                        x = newPos.getX();
                        y = newPos.getY();
                        std::shared_ptr<engine::HandleCreation> creat (new engine::HandleCreation(x,y,state::BuildingType::turret,true));
                        engine.addCommands(creat);
                        engine.execute(state);
                        objSel = nullptr;
                    }

                }

                //barrack create stuff
                if (objSel.get() && objSel.get()->getProperty()->getStringType() == "barrack") {
                    std::cout << "Creation Mode : \nF: new Farmer\nA: new Archer\nS: new Soldier\n" << std::endl;
                    if (event.key.code == sf::Keyboard::F) {
                        unsigned int x;
                        unsigned int y;
                        state::Position newPos;
                        newPos = objSel.get()->getPosition();
                        x = newPos.getX();
                        y = newPos.getY();
                        std::shared_ptr<engine::HandleCreation> creat(
                                new engine::HandleCreation(x, y, state::UnitType::farmer , false));
                        engine.addCommands(creat);
                        engine.execute(state);
                        objSel = nullptr;
                    } else if (event.key.code == sf::Keyboard::A) {
                        unsigned int x;
                        unsigned int y;
                        state::Position newPos;
                        newPos = objSel.get()->getPosition();
                        x = newPos.getX();
                        y = newPos.getY();
                        std::shared_ptr<engine::HandleCreation> creat(
                                new engine::HandleCreation(x, y, state::UnitType::archer , false));
                        engine.addCommands(creat);
                        engine.execute(state);
                        objSel = nullptr;
                    } else if (event.key.code == sf::Keyboard::B) {
                        unsigned int x;
                        unsigned int y;
                        state::Position newPos;
                        newPos = objSel.get()->getPosition();
                        x = newPos.getX();
                        y = newPos.getY();
                        std::shared_ptr<engine::HandleCreation> creat(
                                new engine::HandleCreation(x, y, state::UnitType::infantry, false));
                        engine.addCommands(creat);
                        engine.execute(state);
                        objSel = nullptr;
                    }
                }
                //move on map
                if(event.key.code == sf::Keyboard::Space)
                {
                    endturn = true;
                }
                else if(event.key.code == sf::Keyboard::Up)
                {
                    y_move = 1;

                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    y_move = -1;
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    x_move = -1;
                }
                else if(event.key.code == sf::Keyboard::Left)
                {
                    x_move = 1;
                }
                //stuff
                if(event.key.code == sf::Keyboard::S)
                {
                    engine.addCommands(command_save);
                    engine.execute(state);
                }
            }
            if(event.type == sf::Event::KeyReleased)
            {
                if(event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
                {
                    y_move = 0;
                }
                else if(event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left)
                {
                    x_move = 0;
                }

            }

            if(event.type == sf::Event::Closed)
            {
                exit(0);
            }

        }
        //handle Changes
        sf::Time elapsed1 = clock.getElapsedTime();
        sf::Time inputLock = sf::seconds(0.1f);
        if(elapsed1 < inputLock){
            sf::sleep(inputLock - elapsed1);
        }

        if(x_move || y_move)
        {
            OnMapLayer::scalar_top_right_y += 12*y_move;
            OnMapLayer::scalar_top_right_x += 12*x_move;
            state::Event game_event = state::Event(state::EventTypeId::TERRAIN_CHANGED);
            notifyObservers(game_event);
        }
        clock.restart();

    }
    std::shared_ptr<engine::HandleTurn> commande_turn (new engine::HandleTurn());

    engine.addCommands(commande_turn);
}

void PlayerAction::_getClickLocation (sf::Event event, int * ptr_x, int * ptr_y){
    //on recalcul car ça peut avoir changer au niveau du rendu (nouveau zoom etc)

    int x_cart=0,y_cart=0;
    float x_iso=0, y_iso=0;
    float Tx = (float)(DrawElement::sprite_x + 4 * DrawElement::sprite_x /DrawElement::sprite_y* OnMapLayer::zoom_level)/2;
    float Ty = (float)(DrawElement::sprite_y + 4 * OnMapLayer::zoom_level)/2;
    int OFx = OnMapLayer::scalar_top_right_x;
    int OFy = OnMapLayer::scalar_top_right_y;

    x_cart = event.mouseButton.x;
    y_cart = event.mouseButton.y;

    x_iso = (1 / (2*Tx) * (x_cart + (y_cart-OFy)*Tx/Ty-Tx-OFx));//
    y_iso = ((y_cart-OFy-x_iso*Ty)*1/Ty);

    *ptr_x = (int)x_iso;
    *ptr_y = (int)y_iso;
}