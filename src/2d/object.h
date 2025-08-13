#ifndef OBJECT_H
#define OBJECT_H
#include "util/util.h"

class Object
{
public:
  virtual ~Object() = default;
  void update(GLfloat deltatime)
  {
    center.first += dx * deltatime;
    center.second += dx * deltatime;
    theta = fmod((theta + dtheta * deltatime), 360);
  }

  // Add the vertices of the objects into the VBO
  virtual void draw(bufferObjects *buffers) = 0;

protected:
  point center;
  GLfloat dtheta;
  GLfloat theta;
  GLfloat dx;
  GLfloat dy;
  RGBAColor color;
};

#endif