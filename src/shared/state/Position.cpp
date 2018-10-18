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

bool Position::getPostion(unsigned int *ptr_X, unsigned int *ptr_Y) {
    *ptr_X = positionX;
    *ptr_Y = positionY;
    return true;
}

bool Position::setPosition(unsigned int newX, unsigned int newY) {
    positionX = newX;
    positionY = newY;
}