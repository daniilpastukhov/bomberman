//
// Created by Daniil Pastukhov on 2019-04-25.
//

#include "Bomb.h"
#include "Game.h"

Bomb::Bomb(int _x, int _y, int _t) : Object(_x, _y), explode_time(_t) {}

char Bomb::symbol() const {
    return 'o';
}

Colors Bomb::color() const {
    return COLOR_BOMB;
}

void Bomb::decreaseTimer() {
    explode_time--;
}

int Bomb::getTimer() const {
    return explode_time;
}

void Bomb::detonate(int radius, Game &g, Player &p) {
    if (x == p.getX() && y == p.getY()) g.gameOver();
    explode_time = -1;
    was_detonated = true;
    bool up = true, down = true, right = true, left = true;
    int i = 0;
    while (i++ < radius) {
        attron(COLOR_PAIR(COLOR_BOMB));

        if (right) {
            auto collision = g.checkCollision(x + i, y);
            if (collision) {
                right = false;
                auto object_iterator = find(g.objects.begin(), g.objects.end(), collision);
                if (!checkBombCollision(object_iterator, g, p) && (*object_iterator)->isDestroyable()) {
                    g.objects.erase(object_iterator);
                    p.increaseScore(collision->getPoints());
                }
            }
        }

        if (left) {
            auto collision = g.checkCollision(x - i, y);
            if (collision) {
                left = false;
                auto object_iterator = find(g.objects.begin(), g.objects.end(), collision);
                if (!checkBombCollision(object_iterator, g, p) && (*object_iterator)->isDestroyable()) {
                    g.objects.erase(object_iterator);
                    p.increaseScore(collision->getPoints());
                }
            }
        }

        if (down) {
            auto collision = g.checkCollision(x, y + i);
            if (collision) {
                down = false;
                auto object_iterator = find(g.objects.begin(), g.objects.end(), collision);
                if (!checkBombCollision(object_iterator, g, p) && (*object_iterator)->isDestroyable()) {
                    g.objects.erase(object_iterator);
                    p.increaseScore(collision->getPoints());
                }
            }
        }

        if (up) {
            auto collision = g.checkCollision(x, y - i);
            if (collision) {
                up = false;
                auto object_iterator = find(g.objects.begin(), g.objects.end(), collision);
                if (!checkBombCollision(object_iterator, g, p) && (*object_iterator)->isDestroyable()) {
                    g.objects.erase(object_iterator);
                    p.increaseScore(collision->getPoints());
                }
            }
        }

        if (right) mvaddch(y, x + i, 'f');
        if (left) mvaddch(y, x - i, 'f');
        if (down) mvaddch(y + i, x, 'f');
        if (up) mvaddch(y - i, x, 'f');
    }

    refresh();
}

bool Bomb::checkBombCollision(std::vector<std::shared_ptr<Object>>::iterator &i, Game &g, Player &p) {
    std::shared_ptr<Bomb> b = std::dynamic_pointer_cast<Bomb>(*i);
    if (b && !b->was_detonated) {
        b->detonate(p.getExplosionRange(), g, p);

        auto obj_position = find(g.objects.begin(), g.objects.end(), b);
        if (obj_position != g.objects.end()) g.objects.erase(obj_position);

        auto position = find(p.bombs.begin(), p.bombs.end(), b);
        if (position != p.bombs.end()) p.bombs.erase(position);

        p.decreaseBombsPlaced();
        return true;
    } else if (auto ghost = std::dynamic_pointer_cast<Ghost>(*i)) {
        auto enemy_position = find(g.enemies.begin(), g.enemies.end(), ghost);
        p.increaseScore(ghost->getPoints());
        g.enemies.erase(enemy_position);
    }

    return false;
}