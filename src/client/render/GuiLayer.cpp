//
// Created by ben on 07/01/19.
//

#include "GuiLayer.h"

using namespace render;

GuiLayer::GuiLayer (std::string texture_path):DrawLayer(texture_path)
{
    this->nextLayer = nullptr;

}

void GuiLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &texture_to_apply;
    target.draw(quads,states.texture);

    if(this->nextLayer) {
        this->nextLayer->draw(target, states);
    }
}

bool GuiLayer::loadTexture(const std::string& terrain_file) {
    // load the tileset texture

    return(texture_to_apply.loadFromFile(terrain_file));
}

bool GuiLayer::initQuads(int count) {
    //definit le type de vertex: quads
    quads.setPrimitiveType(sf::Quads);
    //count represente le nombre de la tuile
    quads.resize(4*(size_t) count);
    return true;
}

bool GuiLayer::setSpriteLocation() {
    sf::Vertex* quad = &quads[vertex_count*4];
    return true;
}

bool GuiLayer::setSpriteTexture() {
    sf::Vertex* quad = &quads[vertex_count*4];

    quad[0].texCoords = sf::Vector2f(0, 0);
    quad[1].texCoords = sf::Vector2f(50,0);
    quad[3].texCoords = sf::Vector2f(50,20);
    quad[2].texCoords = sf::Vector2f(0,20);
    return true;
}