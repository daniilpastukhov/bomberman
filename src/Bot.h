//
// Created by Daniil Pastukhov on 2019-05-10.
//

#ifndef BOMBERMAN_BOT_H
#define BOMBERMAN_BOT_H

#include "Player.h"
#include "Game.h"
#include "BombBonus.h"

/**
 * @brief A class to represent a bot.
 */
class Bot : public Player {
public:
    /**
     * @param[in] _x Coordinate x.
     * @param[in] _y Coordinate y.
     */
    Bot(int _x, int _y);

    /**
     * @brief Choose the direction to go.
     * @param[in] g Instance of the game.
     * @param[in] p1 Player who the bomb belongs to.
     */
    void chooseDirection(Game &g, std::shared_ptr<Player> &p1);

    /**
     * @brief Check collisions and move a bot.
     * @param[in] x Coordinate x.
     * @param[in] y Coordinate y.
     * @param[in] g Instance of the game.
     */
    bool moveBot(int x, int y, Game &g);

    /**
     * @brief Try to avoid a bomb.
     * @param[in] g Instance of the game.
     */
    void avoidBomb(Game &g);

    // Getters.
    std::pair<int, int> getBomb() const;

private:
    // std::vector<std::pair<int, int>> movements; // Remember last movements. TODO
    std::pair<int, int> bomb;
};


#endif //BOMBERMAN_BOT_H
