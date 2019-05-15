//
// Created by Daniil Pastukhov on 2019-05-10.
//

#include "Bot.h"

Bot::Bot(int _x, int _y) : Player(_x, _y, 'E') {}

void Bot::chooseDirection(Game &g, std::shared_ptr<Player> &p1) {
    static int moves = 0;
    int diff_x = p1->getX() - x;
    int diff_y = p1->getY() - y;
    int sign_x = (diff_x >= 0) ? 1 : -1;
    int sign_y = (diff_y >= 0) ? 1 : -1;

    if ((sign_x * diff_x == 0 && sign_y * diff_y < 3) || (sign_x * diff_x < 3 && sign_y * diff_y == 0)) {
        placeBomb(g);
        avoidBomb(g);
    } else if (diff_x == 0) {
        if (!moveBot(sign_x * 1, 0, g)) {
            placeBomb(g);
            bomb.first = x;
            bomb.second = y;
            avoidBomb(g);
        }
    } else if (diff_y == 0) {
        if (!moveBot(sign_x * 1, 0, g)) {
            placeBomb(g);
            bomb.first = x;
            bomb.second = y;
            avoidBomb(g);
        }
    } else if (abs(diff_y) < abs(diff_x) && moves % 3 != 0) {
        if (!moveBot(0, sign_y * 1, g)) {
            placeBomb(g);
            bomb.first = x;
            bomb.second = y;
            avoidBomb(g);
        }
    } else {
        if (!moveBot(sign_x * 1, 0, g)) {
            placeBomb(g);
            bomb.first = x;
            bomb.second = y;
            avoidBomb(g);
        }
    }
    avoidBomb(g);
    moves++;
}

bool Bot::moveBot(int x, int y, Game &g) {
    for (auto &i: g.objects) {
        if (getX() + x == i->getX() && getY() + y == i->getY()) {
            if (std::dynamic_pointer_cast<BombBonus>(i)) {
                increaseBombBonus();
                move(x, y);
                g.objects.erase(find(g.objects.begin(), g.objects.end(), i));
            } else if (std::dynamic_pointer_cast<RangeBonus>(i)) {
                increaseRangeBonus();
                move(x, y);
                g.objects.erase(find(g.objects.begin(), g.objects.end(), i));
            } else if (std::dynamic_pointer_cast<Ghost>(i)) {
                move(-x, -y);
                return true;
//                g.gameOver();
            } else return false;
        }
    }
    move(x, y);
    return true;
}

void Bot::avoidBomb(Game &g) {
    if (bombs.size() == 0) return;
    if (bombs[0]->getX() != x && bombs[0]->getY() != y) return;
    for (int i = 0; i < 10; i++) {
        moveBot(0, 1, g);
        if (bombs[0]->getX() != x && bombs[0]->getY() != y) break;
        moveBot(1, 0, g);
        if (bombs[0]->getX() != x && bombs[0]->getY() != y) break;
        moveBot(0, -1, g);
        if (bombs[0]->getX() != x && bombs[0]->getY() != y) break;
        moveBot(-1, 0, g);
        if (bombs[0]->getX() != x && bombs[0]->getY() != y) break;
    }
}

std::pair<int, int> Bot::getBomb() const {
    return bomb;
}
