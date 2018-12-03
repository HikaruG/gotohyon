//
// Created by ben on 23/11/18.
//

#include "OnMapLayer.h"

using namespace render;


unsigned int OnMapLayer::map_size_x;
unsigned int OnMapLayer::map_size_y;


OnMapLayer::OnMapLayer(std::string sprite_path):DrawLayer(sprite_path)
{
    this->zoom_level = 1;
    this->scalar_top_right_x = 0;
    this->scalar_top_right_y = 0;


}


void OnMapLayer::updateElements(std::vector<render::DrawElement> new_list) {

}

void OnMapLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &texture_to_apply;
    target.draw(quads,states.texture);

    if(this->nextLayer)
        this->nextLayer->draw(target, states);

}

bool OnMapLayer::setSpriteLocation (unsigned int count, unsigned int x, unsigned int y)
{

    return true;
}


bool OnMapLayer::updateZoom(unsigned int newZoom) {
    if(newZoom>=1)
        zoom_level=newZoom;
    return true;
}

bool OnMapLayer::updateMapBorder(unsigned int new_x, unsigned int new_y) {
    scalar_top_right_x = new_x;
    scalar_top_right_y = new_y;
}



bool OnMapLayer::change_map_size(unsigned int size_x, unsigned int size_y) {
    OnMapLayer::map_size_x = size_x;
    OnMapLayer::map_size_y = size_y;
    return true;
}


bool OnMapLayer::loadTexture(const std::string& terrain_file) {
    // load the tileset texture
    return(texture_to_apply.loadFromFile(terrain_file));
}


bool OnMapLayer::initQuads(int count) {
    //definit le type de vertex: quads
    quads.setPrimitiveType(sf::Quads);
    //count represente le nombre de la tuile
    quads.resize(4*(size_t) count);
    return true;
}

bool OnMapLayer::setSpriteLocation(unsigned int count, unsigned int x, unsigned int y) {
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

bool OnMapLayer::setSpriteTexture(unsigned int tileset_layer, unsigned int tileset_position_x) {
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