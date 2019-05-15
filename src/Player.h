//
// Created by Daniil Pastukhov on 2019-04-24.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include "Object.h"
#include "Bomb.h"

/**
 * @brief A class to represent a player.
 */
class Player : public Object {
public:
    /**
     * @param[in] _x Coordinate x.
     * @param[in] _y Coordinate y.
     * @param[in] _c Char to represent the player.
     */
    Player(int _x, int _y, char _c);

    char symbol() const override;

    Colors color() const override;

    /**
     * @brief Place bomb on current position.
     * /param[in] g Instance of the game.
     */
    void placeBomb(Game &g);

    /**
     * @brief Increase the duration of bomb bonus.
     */
    void increaseBombBonus();

    /**
     * @brief Increase the duration of range bonus.
     */
    void increaseRangeBonus();

    /**
     * @brief Decrease all player's bombs timer.
     * /param[in] g Instance of the game.
     * @return True if bomb was detonated, false otherwise.
     */
    bool decreaseBombTimer(Game &g);

    /**
     * @brief Decrease the duration of bomb bonus.
     */
    void decreaseBombsPlaced();

    /**
     * @brief Decrease the duration of range bonus.
     */
    void decreaseBonusTimers();

    /**
     * @brief Increase player's score.
     * @param[in] s Number to increase score with.
     */
    void increaseScore(int s);

    // Getters.
    int getExplosionRange() const;

    int getScore() const;

    bool getBonusStatus() const;

    std::vector<std::shared_ptr<Bomb>> bombs; // Player's bombs.

private:
    char id;
    int explosion_range;
    int bombs_placed;
    int max_bombs;
    int score;
    std::pair<bool, int> bomb_bonus;
    std::pair<bool, int> range_bonus;
};

#endif //BOMBERMAN_PLAYER_H
