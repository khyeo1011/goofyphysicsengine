#ifndef CIRCLE_H
#define CIRCLE_H

#include "object.h"

class Circle : public Object {
public:
    Circle(point center = {0, 0}, GLfloat radius = 1, RGBAColor* color = new RGBAColor(1, 1, 1, 1), GLfloat dx = 0, GLfloat dy = 0);

    void draw(bufferObjects* buffers) override;
    void update(GLfloat deltaTime) override;
    bool isCollidingWith(const Object* other) const override;
    bool isCollidingWithRectangle(const class Rectangle* rect) const override;
    bool isCollidingWithCircle(const Circle* circ) const;
    bool isCollidingWithTriangle(const class Triangle* tri) const override;

    GLfloat getRadius() const { return radius; }

private:
    GLfloat radius;
};

#endif
