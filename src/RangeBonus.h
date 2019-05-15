//
// Created by Daniil Pastukhov on 2019-05-07.
//

#ifndef BOMBERMAN_RANGEBONUS_H
#define BOMBERMAN_RANGEBONUS_H

#include "Game.h"
#include "Bonus.h"

/**
 * @brief A class to represent a range bonus.
 */
class RangeBonus : public Bonus {
public:
    /**
     * @param[in] _x Coordinate x.
     * @param[in] _y Coordinate y.
     */
    RangeBonus(int _x, int _y);

    void spawn(Game &) override;

    char symbol() const override;

    Colors color() const override;
};


#endif //BOMBERMAN_RANGEBONUS_H
