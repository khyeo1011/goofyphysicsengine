#ifndef COLLISION_H
#define COLLISION_H

#include "object.h"

class Collision {
public:
    static void handleCollision(Object* objA, Object* objB);
};

#endif
