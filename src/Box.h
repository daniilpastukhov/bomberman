//
// Created by Daniil Pastukhov on 2019-04-24.
//

#ifndef BOMBERMAN_BOX_H
#define BOMBERMAN_BOX_H

#include "Types.h"
#include "Object.h"

/*
 * A class to represent a box.
 */
class Box : public Object {
public:
    /*
     * \param[in] _x Coordinate x.
     * \param[in] _y Coordinate y.
     * \param[in] _b Destructiveness.
     * \param[in] _s Score gain after destruction.
     */
    Box(int _x, int _y, bool _b = true, int _s = 100);

    char symbol() const override;

    Colors color() const override;
};

#endif //BOMBERMAN_BOX_H
