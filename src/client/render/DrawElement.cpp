//
// Created by ben on 28/11/18.
//

#include "DrawElement.h"

using namespace render;

unsigned int DrawElement::sprite_x=64;
unsigned int DrawElement::sprite_y=32;

DrawElement::DrawElement (state::Position position, int id)
{
    this->pos_x = position.getX();
    this->pos_y = position.getY();
    this->sprite_nbr = id;

}
