//
// Created by Daniil Pastukhov on 2019-04-24.
//

#include "Player.h"
#include "Game.h"

Player::Player(int _x, int _y, char _c) : Object(_x, _y), id(_c) {
    explosion_range = EXPLOSION_RANGE;
    bombs_placed = 0;
    max_bombs = BOMBS_MAX;
    bomb_bonus = { false, 0 };
    range_bonus = { false, 0 };
    score = 0;
}

char Player::symbol() const {
    return id;
};

Colors Player::color() const {
    return COLOR_PLAYER;
}

void Player::placeBomb(Game &g) {
    if (bombs_placed < max_bombs) {
        std::shared_ptr<Bomb> bomb(new Bomb(x, y, TICKS_MAX));
        g.objects.push_back(bomb);
        bombs.push_back(bomb);
        bombs_placed++;
    }
}

bool Player::decreaseBombTimer(Game &g) {
    for (auto &bomb: bombs) {
        bomb->decreaseTimer();
        if (bomb->getTimer() == 0) {
            bomb->detonate(explosion_range, g, *this);

            usleep(50000);

            auto obj_position = find(g.objects.begin(), g.objects.end(), bomb);
            if (obj_position != g.objects.end()) g.objects.erase(obj_position);

            auto position = find(bombs.begin(), bombs.end(), bomb);
            if (position != bombs.end()) bombs.erase(position);

            bombs_placed--;
            refresh();
            return true;
        }
    }

    return false;
}

int Player::getExplosionRange() const {
    return explosion_range;
}

void Player::decreaseBombsPlaced() {
    bombs_placed--;
}

void Player::increaseBombBonus() {
    bomb_bonus.first = true;
    bomb_bonus.second += BONUS_DURATION;
    max_bombs = BOMB_BONUS;
}

void Player::increaseRangeBonus() {
    range_bonus.first = true;
    range_bonus.second += BONUS_DURATION;
    explosion_range = RANGE_BONUS;
}

void Player::decreaseBonusTimers() {
    if (bomb_bonus.second > 0 && --bomb_bonus.second == 0) {
        bomb_bonus.first = false;
        max_bombs = BOMBS_MAX;
    }

    if (range_bonus.second > 0 && --range_bonus.second == 0) {
        range_bonus.first = false;
        explosion_range = EXPLOSION_RANGE;
    }
}

bool Player::getBonusStatus() const {
    return bomb_bonus.first || bomb_bonus.second;
}

void Player::increaseScore(int s) {
    score += s;
}

int Player::getScore() const {
    return score;
}
