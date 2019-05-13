//
// Created by Daniil Pastukhov on 2019-04-19.
//

#include "Object.h"

Object::Object(int _x, int _y, bool _d, int _p) : x(_x), y(_y), destroyable(_d), points(_p) {}

void Object::move(int horizontal_change, int vertical_change) {
    x += horizontal_change;
    y += vertical_change;
}

void Object::draw() const {
    attron(COLOR_PAIR(color()));
    mvaddch(y, x, symbol());
}

int Object::getX() const {
    return x;
}

int Object::getY() const {
    return y;
}

bool Object::isDestroyable() const {
    return destroyable;
}

int Object::getPoints() const {
    return points;
}
