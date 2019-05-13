//
// Created by Daniil Pastukhov on 2019-04-24.
//

#ifndef BOMBERMAN_WALL_H
#define BOMBERMAN_WALL_H

#include "Object.h"

/*
 * \brief A class to represent a wall.
 */
class Wall : public Object {
public:
    /*
     * \param[in] _x Coordinate x.
     * \param[in] _y Coordinate y.
     */
    Wall(int _x, int _y);

    char symbol() const override;

    Colors color() const override;
};


#endif //BOMBERMAN_WALL_H
