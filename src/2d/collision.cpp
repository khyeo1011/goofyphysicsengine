#include "collision.h"
#include "triangle.h"
#include "circle.h"

void Collision::handleCollision(Object* objA, Object* objB) {
    if (auto* circA = dynamic_cast<Circle*>(objA)) {
        if (auto* circB = dynamic_cast<Circle*>(objB)) {
            point normal = {circB->center.first - circA->center.first, circB->center.second - circA->center.second};
            float len = std::sqrt(normal.first * normal.first + normal.second * normal.second);
            if (len == 0) return;
            normal.first /= len;
            normal.second /= len;

            float relVelX = circB->dx - circA->dx;
            float relVelY = circB->dy - circA->dy;
            float relVelAlongNormal = relVelX * normal.first + relVelY * normal.second;
            if (relVelAlongNormal > 0) return;

            float mA = circA->mass;
            float mB = circB->mass;
            float e = Object::elasticity;

            float impulse = -(1 + e) * relVelAlongNormal / (1.0f / mA + 1.0f / mB);
            float impulseX = impulse * normal.first;
            float impulseY = impulse * normal.second;

            circA->dx -= impulseX / mA;
            circA->dy -= impulseY / mA;
            circB->dx += impulseX / mB;
            circB->dy += impulseY / mB;
        }
    } else if (auto* triA = dynamic_cast<Triangle*>(objA)) {
        if (auto* triB = dynamic_cast<Triangle*>(objB)) {
            point normal = {triB->center.first - triA->center.first, triB->center.second - triA->center.second};
            float len = std::sqrt(normal.first * normal.first + normal.second * normal.second);
            if (len == 0) return;
            normal.first /= len;
            normal.second /= len;

            float relVelX = triB->dx - triA->dx;
            float relVelY = triB->dy - triA->dy;
            float relVelAlongNormal = relVelX * normal.first + relVelY * normal.second;
            if (relVelAlongNormal > 0) return;

            float mA = triA->mass;
            float mB = triB->mass;
            float e = Object::elasticity;

            float impulse = -(1 + e) * relVelAlongNormal / (1.0f / mA + 1.0f / mB);
            float impulseX = impulse * normal.first;
            float impulseY = impulse * normal.second;

            triA->dx -= impulseX / mA;
            triA->dy -= impulseY / mA;
            triB->dx += impulseX / mB;
            triB->dy += impulseY / mB;
        } else if (auto* circB = dynamic_cast<Circle*>(objB)) {
            point normal = {circB->center.first - triA->center.first, circB->center.second - triA->center.second};
            float len = std::sqrt(normal.first * normal.first + normal.second * normal.second);
            if (len == 0) return;
            normal.first /= len;
            normal.second /= len;

            float relVelX = circB->dx - triA->dx;
            float relVelY = circB->dy - triA->dy;
            float relVelAlongNormal = relVelX * normal.first + relVelY * normal.second;
            if (relVelAlongNormal > 0) return;

            float mA = triA->mass;
            float mB = circB->mass;
            float e = Object::elasticity;

            float impulse = -(1 + e) * relVelAlongNormal / (1.0f / mA + 1.0f / mB);
            float impulseX = impulse * normal.first;
            float impulseY = impulse * normal.second;

            triA->dx -= impulseX / mA;
            triA->dy -= impulseY / mA;
            circB->dx += impulseX / mB;
            circB->dy += impulseY / mB;
        }
    } else if (auto* circA = dynamic_cast<Circle*>(objA)) {
        if (auto* triB = dynamic_cast<Triangle*>(objB)) {
            point normal = {triB->center.first - circA->center.first, triB->center.second - circA->center.second};
            float len = std::sqrt(normal.first * normal.first + normal.second * normal.second);
            if (len == 0) return;
            normal.first /= len;
            normal.second /= len;

            float relVelX = triB->dx - circA->dx;
            float relVelY = triB->dy - circA->dy;
            float relVelAlongNormal = relVelX * normal.first + relVelY * normal.second;
            if (relVelAlongNormal > 0) return;

            float mA = circA->mass;
            float mB = triB->mass;
            float e = Object::elasticity;

            float impulse = -(1 + e) * relVelAlongNormal / (1.0f / mA + 1.0f / mB);
            float impulseX = impulse * normal.first;
            float impulseY = impulse * normal.second;

            circA->dx -= impulseX / mA;
            circA->dy -= impulseY / mA;
            triB->dx += impulseX / mB;
            triB->dy += impulseY / mB;
        }
    }
}
