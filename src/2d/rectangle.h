#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "object.h"
#define NUM_VERTIX_RECT 4
#define NUM_TRIANGLE_RECT 2

class Rectangle : public Object
{
public:
  Rectangle(point center = {0, 0}, GLfloat length = 1, GLfloat width = 1, RGBAColor *color = new RGBAColor(1, 1, 1, 1), GLfloat angle = 0, GLfloat dx = 0, GLfloat dy = 0, GLfloat dtheta = 0)
  {
    std::cout << "[DEBUG] Rectangle color(constructor): R=" << color->r << ", G=" << color->g << ", B=" << color->b << ", A=" << color->a << std::endl;
    this->center = center;
    this->length = length;
    this->width = width;
    this->theta = angle;
    this->dtheta = dtheta;
    this->dx = dx;
    this->dy = dy;
    this->color = color;
    updateVertices();
    for (int i = 0; i < NUM_VERTIX_RECT; i++)
    {
      if ((vertices[i].first <= leftLimit) || (vertices[i].first >= rightLimit) || (vertices[i].second <= downLimit) || (vertices[i].second >= upLimit))
      {
        center.first = 0;
        center.second = 0;
        length = 1;
        width = 1;
      }
    }
  };

  // Add the vertices of the objects into the VBO
  void draw(bufferObjects *buffers) override;
  point getCenter() const { return center; }
  void update(GLfloat deltatime) override;
  bool isCollidingWith(const Object *other) const override;
  bool isCollidingWithRectangle(const Rectangle *rect) const override;
  void updateVertices();

  float getMomentOfInertia() const
  {
    return (1.0f / 12.0f) * mass * (length * length + width * width);
  }

  float getMinX() const
  {
    float minX = vertices[0].first;
    for (int i = 1; i < 4; ++i)
      minX = std::min(minX, vertices[i].first);
    return minX;
  }
  float getMaxX() const
  {
    float maxX = vertices[0].first;
    for (int i = 1; i < 4; ++i)
      maxX = std::max(maxX, vertices[i].first);
    return maxX;
  }
  float getMinY() const
  {
    float minY = vertices[0].second;
    for (int i = 1; i < 4; ++i)
      minY = std::min(minY, vertices[i].second);
    return minY;
  }
  float getMaxY() const
  {
    float maxY = vertices[0].second;
    for (int i = 1; i < 4; ++i)
      maxY = std::max(maxY, vertices[i].second);
    return maxY;
  }

private:
  GLfloat length;
  GLfloat width;
  point vertices[4];
};

#endif