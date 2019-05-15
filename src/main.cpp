//
// Created by Daniil Pastukhov on 2019-04-19.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <unistd.h>
#include <ncurses.h>
#include <string>
#include <sstream>
#include <algorithm>

#include "Types.h"
#include "Object.h"
#include "Wall.h"
#include "Box.h"
#include "Bomb.h"
#include "Player.h"
#include "Game.h"
#include "Bonus.h"
#include "BombBonus.h"
#include "RangeBonus.h"
#include "Ghost.h"
#include "Bot.h"

/**
 * @mainpage Bomberman
 * @section desc_sec Description
 * An implementation of Bomberman (https://en.wikipedia.org/wiki/Bomberman) using C++ with ncurses library.
 */

int main() {
    std::shared_ptr<Game> game = std::make_shared<Game>();

    game->init();
    game->run();
    game->close();

    return 0;
}