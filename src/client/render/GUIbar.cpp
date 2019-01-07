//
// Created by ben on 07/01/19.
//

#include "GUIbar.h"

using namespace render;

GUIbar::GUIbar (int pos_x, int pos_y, int size_x, int size_y, std::string texture_path):DrawLayer(texture_path)
{
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->size_x = size_x;
    this->size_y = size_y;
    this->nextLayer = nullptr;

}

void GUIbar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &texture_to_apply;
    target.draw(quads,states.texture);
    if(this->nextLayer) {
        this->nextLayer->draw(target, states);
    }
}

bool GUIbar::loadTexture(const std::string& terrain_file) {
    // load the tileset texture

    return(texture_to_apply.loadFromFile(terrain_file));
}

bool GUIbar::initQuads(int count) {
    //definit le type de vertex: quads
    quads.setPrimitiveType(sf::Quads);
    //count represente le nombre de la tuile
    quads.resize(4*(size_t) count);
    return true;
}

bool GUIbar::setSpriteLocation() {
    sf::Vertex* quad = &quads[vertex_count*4];
    return true;
}

bool GUIbar::setSpriteTexture() {
    sf::Vertex* quad = &quads[vertex_count*4];

    quad[0].texCoords = sf::Vector2f(0, 0);
    quad[1].texCoords = sf::Vector2f(50,0);
    quad[3].texCoords = sf::Vector2f(50,20);
    quad[2].texCoords = sf::Vector2f(0,20);
    return true;
}