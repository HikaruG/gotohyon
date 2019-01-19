//
// Created by ben on 28/11/18.
//

#include "DrawElement.h"

using namespace render;

unsigned int DrawElement::sprite_x=256;
unsigned int DrawElement::sprite_y=128;

DrawElement::DrawElement (state::Position position, int id, int player_id)
{
    this->pos_x = position.getX();
    this->pos_y = position.getY();
    this->sprite_nbr = id;
    this->sprite_player_id = player_id;

}
