//
// Created by Daniil Pastukhov on 2019-04-24.
//

#include "Wall.h"

Wall::Wall(int _x, int _y) : Object(_x, _y, false) {}

char Wall::symbol() const {
    return 'w';
}

Colors Wall::color() const {
    return COLOR_WALL;
}