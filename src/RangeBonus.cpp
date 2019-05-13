//
// Created by Daniil Pastukhov on 2019-05-07.
//

#include "RangeBonus.h"

RangeBonus::RangeBonus(int _x, int _y) : Bonus(_x, _y) {}

void RangeBonus::spawn(Game &g) {
    int pos_x = rand() % 78 + 1;
    int pos_y = rand() % 22 + 1;

    while (g.checkCollision(pos_x, pos_y)) {
        pos_x = rand() % 78 + 1;
        pos_y = rand() % 22 + 1;
    }

    g.objects.push_back(std::make_shared<RangeBonus>(pos_x, pos_y));
}

char RangeBonus::symbol() const {
    return 'R';
}

Colors RangeBonus::color() const {
    return COLOR_RANGE_BONUS;
}
