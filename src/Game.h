//
// Created by Daniil Pastukhov on 2019-04-19.
//

#ifndef BOMBERMAN_GAME_H
#define BOMBERMAN_GAME_H

#include <fstream>
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <ncurses.h>
#include <algorithm>
#include "Object.h"
#include "Box.h"
#include "Wall.h"
#include "Player.h"
#include "Bot.h"
#include "BombBonus.h"
#include "RangeBonus.h"
#include "Ghost.h"

/*
 * Main class that represents a game itself.
 */
class Game {
public:
    Game();

    /*
     * \brief Initialization of the game.
     */
    void init();

    /*
     * \brief Infinite loop, all processes run here.
     */
    void run();

    /*
     * \brief Close window, called at the end of the game.
     */
    void close();

    /*
     * \brief Load map from a file.
    */
    void loadMap();

    /*
     * \brief Make borders and push them to objects.
     */
    void makeBorders();

    /*
     * \brief Push objects from template to objects.
     */
    void drawMap();

    /*
     * \brief Refresh all objects on the map.
     */
    void refreshMap();

    /*
     * \brief Handle pressed key and do corresponding action.
     * \return true if an action was done and false if no action was done (because of collision).
     */
    bool handlePressedKey(int);

    /*
     * \brief Make move with collision checking.
     * \return true if there is no collisions or there is collision with a bonus and false if there is a collision.
     */
    bool makeMove(int, int, std::shared_ptr<Player> &);

    /*
     * \brief Spawn bomb bonus at random location.
     */
    void spawnBombBonus();

    /*
     * \brief Spawn range bonus at random location.
     */
    void spawnRangeBonus();

    /*
     * \brief Spawn enemy at random location.
     */
    void spawnEnemy();

    /*
     * \brief All enemies make a random step.
     */
    void moveEnemies();

    /*
     * \brief Check collision and make a step.
     */
    void makeEnemyStep(std::shared_ptr<Ghost> &, int, int);

    // TODO
//    void askName(int);

    /*
     * \brief Show game over screen and wait for a quiting.
     */
    void gameOver();

    /*
     * \brief Check collision with any object.
     * \return smart pointer to the object or nullptr.
     */
    std::shared_ptr<Object> checkCollision(int, int) const;

    std::vector<std::shared_ptr<Object>> objects; // Vector with all objects.

    std::vector<std::shared_ptr<Ghost>> enemies; // Vector with all enemies (ghosts).
private:
    Mode gameMode; // Versus bot or versus player.
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
    std::shared_ptr<Bot> bot;
    bool running;
};


#endif //BOMBERMAN_GAME_H
