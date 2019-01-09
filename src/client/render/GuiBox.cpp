//
// Created by ben on 07/01/19.
//
/*
Text.SetColor(sf::Color(128, 128, 0));
Text.SetRotation(90.f);
Text.SetScale(2.f, 2.f);
Text.Move(100.f, 200.f);*/


#include "GuiBox.h"
#include "render.h"
#include <string>

using namespace render;

/*
 *
 *     this->text.push_back(GuiBox("test",0,0,100,50));
    this->debugText.push_back(sf::Text());
    debugText[0].setString("debug_test");
    debugText[0].setFont(font);
    debugText[0].setPosition(10,10);
    debugText[0].setCharacterSize(24); // in pixels, not points!
    debugText[0].setFillColor(sf::Color::Red);
 */



GuiBox::GuiBox (std::string text, int pos_x, int pos_y, int size)
{
    this->text = text;
    this->size=size;
    this->pos_y=pos_y;
    this->pos_x = pos_x;

}
