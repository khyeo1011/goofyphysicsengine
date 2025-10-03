#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"

class Triangle : public Object {
public:
    Triangle() = default;
    Triangle(point v1, point v2, point v3, RGBAColor* color);

    void draw(bufferObjects* buffers) override;
    void update(GLfloat deltaTime) override;
    bool isCollidingWith(const Object* other) const override;
    bool isCollidingWithRectangle(const class Rectangle* rect) const override;
    bool isCollidingWithCircle(const class Circle* circ) const override;
    bool isCollidingWithTriangle(const Triangle* tri) const;

    point vertices[3];
};

#endif
