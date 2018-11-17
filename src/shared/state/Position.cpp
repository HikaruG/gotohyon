//
// Created by ben on 15/10/18.
//
#include "Position.h"

using namespace state;

Position::Position() {
    //default position
    this->positionX = 0;
    this->positionY = 0;
}

Position::Position(unsigned int X, unsigned int Y) {
    this->positionX = X;
    this->positionY = Y;
}


bool Position::setPosition(unsigned int newX, unsigned int newY) {
    positionX = newX;
    positionY = newY;
    return true;
}

bool Position::operator==(const state::Position orig_pos) {
    return orig_pos.positionX == this->positionX && orig_pos.positionY == this->positionY;
}

unsigned int Position::getX()
{
    return this->positionX;
}

unsigned int Position::getY()
{
    return this->positionY;
}