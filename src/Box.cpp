//
// Created by Daniil Pastukhov on 2019-04-24.
//

#include "Box.h"

Box::Box(int _x, int _y, bool _b, int _s) : Object(_x, _y, _b, _s) {}

char Box::symbol() const {
    return 'x';
}

Colors Box::color() const {
    return COLOR_BOX;
}