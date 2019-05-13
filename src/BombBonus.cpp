//
// Created by Daniil Pastukhov on 2019-05-06.
//

#include "BombBonus.h"

BombBonus::BombBonus(int _x, int _y) : Bonus(_x, _y) {}

void BombBonus::spawn(Game &g) {
    int pos_x = rand() % 78 + 1;
    int pos_y = rand() % 22 + 1;

    while (g.checkCollision(pos_x, pos_y)) {
        pos_x = rand() % 78 + 1;
        pos_y = rand() % 22 + 1;
    }

    g.objects.push_back(std::make_shared<BombBonus>(pos_x, pos_y));
}

char BombBonus::symbol() const {
    return 'B';
}

Colors BombBonus::color() const {
    return COLOR_BOMB_BONUS;
}
