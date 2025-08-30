#ifndef TWODSIMULATOR_H
#define TWODSIMULATOR_H
#include <vector>
#include "object.h"

class Simulator2D {
public:
    std::vector<Object*> objects;
    double fG = 1000;
    void update(double dt);
    void handleCollisions();
    void drawAll(bufferObjects* buffers);
    ~Simulator2D();


};

#endif