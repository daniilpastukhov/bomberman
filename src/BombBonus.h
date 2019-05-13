//
// Created by Daniil Pastukhov on 2019-05-06.
//

#ifndef BOMBERMAN_BOMBBONUS_H
#define BOMBERMAN_BOMBBONUS_H

#include "Bonus.h"
#include "Game.h"
#include <random>

/*
 * \brief A class to represent a bomb bonus.
 */
class BombBonus : public Bonus {
public:
    /*
     * \param[in] _x Coordinate x.
     * \param[in] _y Coordinate x.
     */
    BombBonus(int, int);

    void spawn(Game &) override;

    char symbol() const override;

    Colors color() const override;
};


#endif //BOMBERMAN_BOMBBONUS_H
