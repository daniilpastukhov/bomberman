//
// Created by Daniil Pastukhov on 2019-04-25.
//

#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H

//#include "Types.h"
#include "Object.h"
#include "Player.h"
#include <unistd.h>
#include <ncurses.h>
#include <algorithm>

/*
 * \brief A class to represent a bomb.
 */
class Bomb : public Object {
public:
    /*
     * \param[in] _x Coordinate x.
     * \param[in] _y Coordinate y.
     * \param[in] _t Explode time.
     */
    Bomb(int _x, int _y, int _t);

    char symbol() const override;

    Colors color() const override;

    /*
     * \brief Detonate a bomb and destroy all objects it collides with.
     * \param[in] radius Radius of explosion.
     * \param[in] g Instance of the game.
     * \param[in] p Player who the bomb belongs to.
     */
    void detonate(int radius, Game &g, Player &p);

    /*
     * \brief Decrease bomb timer.
     */
    void decreaseTimer();

    /*
     * \brief Check collision with a bomb.
     * \param[in] i Iterator to the object from game class.
     * \param[in] g Instance of the game.
     * \param[in] p Player who the bomb belongs to.
     * \return True if another bomb was detonated, false otherwise.
     */
    bool checkBombCollision(std::vector<std::shared_ptr<Object>>::iterator &i, Game &g, Player &p);

    // Getters
    int getTimer() const;

private:
    int explode_time;
    bool was_detonated;
};


#endif //BOMBERMAN_BOMB_H
