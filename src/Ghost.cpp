//
// Created by Daniil Pastukhov on 2019-05-08.
//

#include "Ghost.h"

Ghost::Ghost(int _x, int _y, bool _b, int _s) : Object(_x, _y, _b, _s) {}

char Ghost::symbol() const {
    return 'g';
}

Colors Ghost::color() const {
    return COLOR_GHOST;
}