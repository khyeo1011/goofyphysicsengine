#include "rectangle.h"
#include "circle.h"
#include "triangle.h"

Rectangle::Rectangle(point v1, point v2, point v3, point v4, RGBAColor *color, GLfloat angle, GLfloat dx, GLfloat dy, GLfloat dtheta) {
    this->t1 = Triangle(v1, v2, v3, color);
    this->t2 = Triangle(v1, v3, v4, color);
    this->theta = angle;
    this->dtheta = dtheta;
    this->dx = dx;
    this->dy = dy;
    this->color = color;
}

bool Rectangle::isCollidingWithRectangle(const Rectangle *rect) const
{
    return t1.isCollidingWith(&rect->t1) || t1.isCollidingWith(&rect->t2) || t2.isCollidingWith(&rect->t1) || t2.isCollidingWith(&rect->t2);
}

bool Rectangle::isCollidingWith(const Object *other) const
{
  return other->isCollidingWithRectangle(this);
}

bool Rectangle::isCollidingWithCircle(const Circle* circ) const {
    return t1.isCollidingWith(circ) || t2.isCollidingWith(circ);
}

bool Rectangle::isCollidingWithTriangle(const Triangle *tri) const {
    return t1.isCollidingWith(tri) || t2.isCollidingWith(tri);
}

void Rectangle::draw(bufferObjects *buffers)
{
    t1.draw(buffers);
    t2.draw(buffers);
}

void Rectangle::update(GLfloat deltaTime)
{
    for (int i = 0; i < 3; i++) {
        if ((t1.vertices[i].first + dx * deltaTime <= leftLimit) || (t1.vertices[i].first + dx * deltaTime >= rightLimit)) {
            dx = -dx * elasticity;
        }
        if ((t1.vertices[i].second + dy * deltaTime <= downLimit) || (t1.vertices[i].second + dy * deltaTime >= upLimit)) {
            dy = -dy * elasticity;
        }
        if ((t2.vertices[i].first + dx * deltaTime <= leftLimit) || (t2.vertices[i].first + dx * deltaTime >= rightLimit)) {
            dx = -dx * elasticity;
        }
        if ((t2.vertices[i].second + dy * deltaTime <= downLimit) || (t2.vertices[i].second + dy * deltaTime >= upLimit)) {
            dy = -dy * elasticity;
        }
    }

    center.first = (t1.vertices[0].first + t1.vertices[1].first + t1.vertices[2].first + t2.vertices[2].first) / 4.0f;
    center.second = (t1.vertices[0].second + t1.vertices[1].second + t1.vertices[2].second + t2.vertices[2].second) / 4.0f;

    for (int i = 0; i < 3; i++) {
        t1.vertices[i].first += dx * deltaTime;
        t1.vertices[i].second += dy * deltaTime;
        t2.vertices[i].first += dx * deltaTime;
        t2.vertices[i].second += dy * deltaTime;
    }

    for (int i = 0; i < 3; i++) {
        float tempX1 = t1.vertices[i].first - center.first;
        float tempY1 = t1.vertices[i].second - center.second;

        float rotatedX1 = tempX1 * cos(dtheta * deltaTime) - tempY1 * sin(dtheta * deltaTime);
        float rotatedY1 = tempX1 * sin(dtheta * deltaTime) + tempY1 * cos(dtheta * deltaTime);

        t1.vertices[i].first = rotatedX1 + center.first;
        t1.vertices[i].second = rotatedY1 + center.second;

        float tempX2 = t2.vertices[i].first - center.first;
        float tempY2 = t2.vertices[i].second - center.second;

        float rotatedX2 = tempX2 * cos(dtheta * deltaTime) - tempY2 * sin(dtheta * deltaTime);
        float rotatedY2 = tempX2 * sin(dtheta * deltaTime) + tempY2 * cos(dtheta * deltaTime);

        t2.vertices[i].first = rotatedX2 + center.first;
        t2.vertices[i].second = rotatedY2 + center.second;
    }
}