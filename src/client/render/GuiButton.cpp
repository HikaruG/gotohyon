//
// Created by ben on 19/12/18.
//

#include "GuiButton.h"

using namespace render;

GuiButton::GuiButton (int pos_x, int pos_y, int size_x, int size_y, std::string path_text, std::string text):DrawLayer(path_text)
{
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->size_x = size_x;
    this->size_y = size_y;
    this->text = text;
    this->nextLayer = nullptr;

}

void GuiButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &texture_to_apply;
    target.draw(quads,states.texture);
    if(this->nextLayer) {
        this->nextLayer->draw(target, states);
    }
}

bool GuiButton::loadTexture(const std::string& terrain_file) {
    // load the tileset texture

    return(texture_to_apply.loadFromFile(terrain_file));
}

bool GuiButton::initQuads(int count) {
    //definit le type de vertex: quads
    quads.setPrimitiveType(sf::Quads);
    //count represente le nombre de la tuile
    quads.resize(4*(size_t) count);
    return true;
}

bool GuiButton::setSpriteLocation() {
    sf::Vertex* quad = &quads[vertex_count*4];
    return true;
}

bool GuiButton::setSpriteTexture() {
    sf::Vertex* quad = &quads[vertex_count*4];

    quad[0].texCoords = sf::Vector2f(0, 0);
    quad[1].texCoords = sf::Vector2f(50,0);
    quad[3].texCoords = sf::Vector2f(50,20);
    quad[2].texCoords = sf::Vector2f(0,20);
    return true;
}