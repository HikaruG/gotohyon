//
// Created by ben on 28/11/18.
//

#include "DrawElement.h"

using namespace render;

DrawElement::DrawElement (state::Position position, int id)
{
    this->pos_x = position.getX();
    this->pos_y = position.getY();
    this->sprite_nbr = id;

    this->sprite_x = 64;
    this->sprite_y = 32;

}
