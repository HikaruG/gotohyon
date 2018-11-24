//
// Created by Hikaru on 24/10/2018.
//

#include "MapSurface.h"
#include <iostream>

using namespace render;

void MapSurface::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    states.texture = &texture_to_apply;
    target.draw(quads,states.texture);
}

bool MapSurface::loadTextures(const std::string& terrain_file) {
    // load the tileset texture
    return(texture_to_apply.loadFromFile(terrain_file));
}


bool MapSurface::initQuads(int count) {
    //definit le type de vertex: quads
    quads.setPrimitiveType(sf::Quads);
    //count represente le nombre de la tuile
    quads.resize(4*(size_t) count);
    return true;
}

bool MapSurface::setSpriteLocation(int count, int x, int y) {
    sf::Vertex* quad = &quads[count*4];
    int offset = 480; // taille map * taille texture longueur, pour eviter d'arriver dans les negatifs
    int t_map_x = 64;
    int t_map_y = 32;
    int cc = -(y * t_map_x);
    int k = (x * t_map_x)/2 + cc/2 + t_map_x/2 + offset;
    int l = ((x + y) * t_map_y) / 2;
    //std::cout<<"Debug : adding sprite nbr "<<count<<" in "<<x<<" "<<y<<" : "<<k<<" "<<l<<std::endl;
    quad[0].position = sf::Vector2f(k, l);
    quad[1].position = sf::Vector2f(k + t_map_x/2, l + t_map_y/2);
    quad[3].position = sf::Vector2f(k - t_map_x/2, l + t_map_y /2);
    quad[2].position = sf::Vector2f(k , l + t_map_y);
    return true;
}

bool const MapSurface::setSpriteTexture(unsigned int tileset_layer, unsigned int tileset_position_x, unsigned int count) {
    sf::Vertex* quad = &quads[count*4];
    int t_map_x = 64;
    int t_map_y = 32;
    int tu = (tileset_position_x) % (texture_to_apply.getSize().x / t_map_x);

    int ku = tu*t_map_x + t_map_x/2;
    //std::cout<<"Debug : adding sprite on layer "<<tileset_layer<<" tile id "<<tileset_position_x<<" count "<<count<<std::endl;

    quad[0].texCoords = sf::Vector2f(ku, 0);
    quad[1].texCoords = sf::Vector2f(ku + t_map_x/2, t_map_y/2);
    quad[3].texCoords = sf::Vector2f(ku - t_map_x/2, t_map_y/2);
    quad[2].texCoords = sf::Vector2f(ku , t_map_y);
    return true;
}
