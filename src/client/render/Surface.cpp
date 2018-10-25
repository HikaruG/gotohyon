//
// Created by Hikaru on 24/10/2018.
//

#include "Surface.h"

using namespace render;

bool const Surface::draw(sf::RenderTarget &target, sf::RenderStates states) {
    //states.transform *= getTransform();
    //aplly the texture
    states.texture = &texture;
    //draw the vertex arrow
    target.draw(quads,texture);
    return true;
}

bool Surface::loadTexture(const std::string &image_file) {
    // load the tileset texture
    if (!texture.loadFromFile(image_file)) {
        return false;
    }
    return true;
}


bool Surface::initQuads(int count) {
    //definit le type de vertex: quads
    quads.setPrimitiveType(sf::Quads);
    //count represente le nombre de la tuile
    quads.resize(4*count);
    return true;
}

bool Surface::setSpriteLocation(int i, int x, int y) {
    //i represente le niveau/level
    //x represente l'emplacement en x
    //y represente l'emplacement en y

    quad[0].position = sf::Vector2f(k, l);
    quad[1].position = sf::Vector2f(k + t_map.x/2, l + t_map.y/2);
    quad[3].position = sf::Vector2f(k - t_map.x/2, l + t_map.y /2);
    quad[2].position = sf::Vector2f(k , l + t_map.y);

}

bool Surface::setSpriteTexture(int i, const render::Tile &text) {


}