#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "object.h"
#include "triangle.h"

class Rectangle : public Object
{
public:
  Rectangle(point v1, point v2, point v3, point v4, RGBAColor *color = new RGBAColor(1, 1, 1, 1), GLfloat angle = 0, GLfloat dx = 0, GLfloat dy = 0, GLfloat dtheta = 0);

  // Add the vertices of the objects into the VBO
  void draw(bufferObjects *buffers) override;
  point getCenter() const { return center; }
  void update(GLfloat deltatime) override;
  bool isCollidingWith(const Object *other) const override;
  bool isCollidingWithRectangle(const Rectangle *rect) const override;
  bool isCollidingWithCircle(const class Circle *circ) const override;
  bool isCollidingWithTriangle(const class Triangle *tri) const override;

  float getMomentOfInertia() const
  {
    return 0; // TODO: Implement
  }

  float getMinX() const
  {
      return 0; // TODO: Implement
  }
  float getMaxX() const
  {
      return 0; // TODO: Implement
  }
  float getMinY() const
  {
      return 0; // TODO: Implement
  }
  float getMaxY() const
  {
      return 0; // TODO: Implement
  }

private:
  Triangle t1;
  Triangle t2;
};

#endif