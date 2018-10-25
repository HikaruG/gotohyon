//
// Created by Hikaru on 24/10/2018.
//

#include "MapSurface.h"

using namespace render;

void MapSurface::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    //states.transform *= getTransform();
    //aplly the texture
//    states.texture = &textures;
//    //draw the vertex arrow
//    target.draw(quads,textures);
}

bool MapSurface::loadTextures(const std::string &image_file) {
    // load the tileset texture
    for(unsigned int i = 0; i<textures.size();i++) {
        if (!textures[i].loadFromFile(image_file)) {
            return false;
        }
    }
    return true;
}


bool MapSurface::initQuads(int count) {
    //definit le type de vertex: quads
    quads.setPrimitiveType(sf::Quads);
    //count represente le nombre de la tuile
    quads.resize(4*count);
    return true;
}

bool MapSurface::setSpriteLocation(int x, int y) {
    //i represente le niveau/level
    //x represente l'emplacement en x
    //y represente l'emplacement en y

//    quads[0].position = sf::Vector2f(k, l);
//    quads[1].position = sf::Vector2f(k + t_map.x/2, l + t_map.y/2);
//    quads[3].position = sf::Vector2f(k - t_map.x/2, l + t_map.y /2);
//    quads[2].position = sf::Vector2f(k , l + t_map.y);

}

bool const MapSurface::setSpriteTexture(const Tile &text) {

    return true;
}
