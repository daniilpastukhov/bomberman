//
// Created by Daniil Pastukhov on 2019-05-06.
//

#ifndef BOMBERMAN_BONUS_H
#define BOMBERMAN_BONUS_H

#include "Object.h"

/**
 * @brief An abstract class to represent bonus.
 */

class Bonus : public Object {
public:
    Bonus(int, int);

    /**
     * @brief Spawn bonus at random location.
     */
    virtual void spawn(Game &) = 0;
};

#endif //BOMBERMAN_BONUS_H
