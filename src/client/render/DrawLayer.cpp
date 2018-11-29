//
// Created by ben on 28/11/18.
//

#include "DrawLayer.h"

using namespace render;

DrawLayer::DrawLayer (std::string sprite_path)
{
    this->spriteFilePath = sprite_path;

}


bool DrawLayer::setNext (DrawLayer& next_layer){
    this->nextLayer = &next_layer;
}

DrawLayer* DrawLayer::getNext ()
{
    return this->nextLayer;
}

