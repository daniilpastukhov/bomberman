//
// Created by Daniil Pastukhov on 2019-05-08.
//

#ifndef BOMBERMAN_GHOST_H
#define BOMBERMAN_GHOST_H

#include "Object.h"

/*
 * \brief A class to represent a ghost (enemy).
 */
class Ghost : public Object {
public:
    /*
     * \param[in] _x Coordinate x.
     * \param[in] _y Coordinate y.
     * \param[in] _b Destructiveness.
     * \param[in] _s Score gain after destruction.
     */
    Ghost(int _x, int _y, bool _b = true, int _s = 200);

    char symbol() const override;

    Colors color() const override;
};


#endif //BOMBERMAN_GHOST_H
