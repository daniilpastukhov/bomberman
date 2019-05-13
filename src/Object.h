//
// Created by Daniil Pastukhov on 2019-04-19.
//

#ifndef BOMBERMAN_OBJECT_H
#define BOMBERMAN_OBJECT_H

#include <memory>
#include <list>
#include <vector>
#include <ncurses.h>
#include "Types.h"

/*
 * \brief An abstract class to represent the object.
 */

class Object {
public:
    /*
     * \param[in] _x Coordinate x.
     * \param[in] _y Coordinate y.
     * \param[in] _d Is destroyable.
     * \param[in] _p Score gain after destruction.
     */
    Object(int _x, int _y, bool _d = true, int _p = 0);

    virtual ~Object() = default;

    /*
     * \brief Returns object's symbol.
     * \return Object's symbol.
     */
    virtual char symbol() const = 0;

    /*
     * \brief Returns object's color.
     * \return Object's color.
     */
    virtual Colors color() const = 0;

    /*
     * \brief Move object.
     * \param[in] horizontal_change X-axis shift.
     * \param[in] vertical_change Y-axis shift.
     */
    void move(int, int);

    /*
     * \brief Draw current object.
     */
    void draw() const;

    // Getters
    int getX() const;

    int getY() const;

    bool isDestroyable() const;

    int getPoints() const;

protected:
    int x, y;
    bool destroyable;
    int points;
};

#endif //BOMBERMAN_OBJECT_H
