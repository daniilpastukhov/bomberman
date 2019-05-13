//
// Created by Daniil Pastukhov on 2019-04-25.
//

#ifndef BOMBERMAN_TYPES_H
#define BOMBERMAN_TYPES_H

#include <string>

static const int SCREEN_WIDTH = 80;
static const int SCREEN_HEIGHT = 24;
static const std::string path = "src/map/map.txt";
//static const std::string path = "src/map/empty_map.txt";

static const int TICKS_MAX = 500000;
//static const int TICKS_MAX = 1500000;

static const int EXPLOSION_RANGE = 3;
static const int BOMBS_MAX = 1;
static const int BONUS_DURATION = 5000000;
static const int GHOST_AMOUNT = 5;

static char map[SCREEN_HEIGHT][SCREEN_WIDTH];

enum Colors {
    COLOR_WALL = 1,
    COLOR_BOX,
    COLOR_PLAYER,
    COLOR_SCORE,
    COLOR_BOMB,
    COLOR_BOMB_BONUS,
    COLOR_RANGE_BONUS,
    COLOR_GHOST
};

enum Bonuses {
    BOMB_BONUS = 3,
    RANGE_BONUS = 5
};

enum Mode {
    VSPLAYER,
    VSBOT
};

// Forward declaration
class Game;
class Player;
class Bomb;
class Bot;

#endif //BOMBERMAN_TYPES_H
