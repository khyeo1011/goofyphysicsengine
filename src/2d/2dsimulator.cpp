#include "2dsimulator.h"
#include "rectangle.h"
#include <iostream>
#include <cmath>

namespace {
    // Helper for collision response
    point computeCollisionNormal(const Rectangle* rectA, const Rectangle* rectB) {
        float dx = rectB->getCenter().first - rectA->getCenter().first;
        float dy = rectB->getCenter().second - rectA->getCenter().second;
        float len = std::sqrt(dx*dx + dy*dy);
        if (len == 0) return {1, 0};
        return {dx / len, dy / len};
    }
}

void Simulator2D::update(double dt) {
    for (auto& obj : objects) {
        obj->update(dt);
        obj->applyGravity(dt * fG);
    }
    handleCollisions();
}

void Simulator2D::handleCollisions() {
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (objects[i]->isCollidingWith(objects[j])) {
                // Only handle Rectangle-Rectangle for now
                Rectangle* rectA = dynamic_cast<Rectangle*>(objects[i]);
                Rectangle* rectB = dynamic_cast<Rectangle*>(objects[j]);
                if (!rectA || !rectB) continue;

                point normal = computeCollisionNormal(rectA, rectB);
                float relVelX = rectB->dx - rectA->dx;
                float relVelY = rectB->dy - rectA->dy;
                float relVelAlongNormal = relVelX * normal.first + relVelY * normal.second;
                if (relVelAlongNormal > 0) continue;

                float impulse = -2 * relVelAlongNormal / 2;
                float impulseX = impulse * normal.first;
                float impulseY = impulse * normal.second;

                rectA->dx -= impulseX;
                rectA->dy -= impulseY;
                rectB->dx += impulseX;
                rectB->dy += impulseY;

                float contactX = (rectA->getCenter().first + rectB->getCenter().first) / 2.0f;
                float contactY = (rectA->getCenter().second + rectB->getCenter().second) / 2.0f;
                float rAx = contactX - rectA->getCenter().first;
                float rAy = contactY - rectA->getCenter().second;
                float rBx = contactX - rectB->getCenter().first;
                float rBy = contactY - rectB->getCenter().second;
                float torqueA = rAx * impulseY - rAy * impulseX;
                float torqueB = rBx * (-impulseY) - rBy * (-impulseX);

                rectA->dtheta += 0.001f * torqueA;
                rectB->dtheta += 0.001f * torqueB;
            }
        }
    }
}

void Simulator2D::drawAll(bufferObjects* buffers) {
    for (auto& obj : objects) {
        obj->draw(buffers);
    }
}

Simulator2D::~Simulator2D() {
    for (auto& obj : objects) {
        delete obj;
    }
}

