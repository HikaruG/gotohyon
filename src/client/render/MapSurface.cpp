//
// Created by Hikaru on 24/10/2018.
//

#include "MapSurface.h"
#include <iostream>

using namespace render;

void MapSurface::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    //states.transform *= getTransform();
    //aplly the texture
    states.texture = &texture_to_apply;
    //draw the vertex arrow
    target.draw(quads,states.texture);
}

bool MapSurface::loadTextures(const std::string& terrain_file, const std::string& unit_file, const std::string& building_file) {
    // load the tileset texture
    if (!texture_terrain.loadFromFile(terrain_file)) {
        return false;
    }
    if (!texture_building.loadFromFile(building_file)) {
        return false;
    }
    if (!texture_unit.loadFromFile(unit_file)) {
        return false;
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

bool MapSurface::setSpriteLocation(int count, int x, int y) {
    sf::Vertex* quad = &quads[count*4];
    int offset = 128; // taille map * taille texture longueur, pour eviter d'arriver dans les negatifs
    int t_map_x = 64;
    int t_map_y = 32;
    int cc = -(x * t_map_x);
    int k = (x * t_map_x)/2 + cc/2 + t_map_x/2 + offset;
    int l = ((x + y) * t_map_y) / 2;

    quads[0].position = sf::Vector2f(k, l);
    quads[1].position = sf::Vector2f(k + t_map_x/2, l + t_map_y/2);
    quads[3].position = sf::Vector2f(k - t_map_x/2, l + t_map_y /2);
    quads[2].position = sf::Vector2f(k , l + t_map_y);

}

bool const MapSurface::setSpriteTexture(unsigned int tileset_layer, unsigned int tileset_position_x) {

    int t_map_x = 64;
    int t_map_y = 32;
    std::vector<sf::Texture> texture_level = {texture_terrain,texture_building,texture_unit};
    texture_to_apply = texture_level[tileset_layer];
    int tu = tileset_position_x % (texture_to_apply.getSize().x / t_map_x);

    int ku = tu*t_map_x + t_map_x/2;

    quads[0].texCoords = sf::Vector2f(ku, 0);
    quads[1].texCoords = sf::Vector2f(ku + t_map_x/2, t_map_y/2);
    quads[3].texCoords = sf::Vector2f(ku - t_map_x/2, t_map_y/2);
    quads[2].texCoords = sf::Vector2f(ku , t_map_y);
    return true;
}
