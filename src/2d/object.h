#ifndef OBJECT_H
#define OBJECT_H
#include "util/util.h"

class Object
{
public:
  static float leftLimit;
  static float rightLimit;
  static float upLimit;
  static float downLimit;
  static float elasticity;
  virtual ~Object() = default;
  virtual void update(GLfloat deltaTime) = 0;

  // Add the vertices of the objects into the VBO
  virtual void draw(bufferObjects *buffers) = 0;

  static void defineLimits(int screenWidth, int screenHeight)
  {
    leftLimit = -(float)screenWidth / 2.0;
    rightLimit = (float)screenWidth / 2.0;
    upLimit = (float)screenHeight / 2.0;
    downLimit = -(float)screenHeight / 2.0;
  }

protected:
  point center;
  GLfloat dtheta;
  GLfloat theta;
  GLfloat dx;
  GLfloat dy;
  RGBAColor *color;
};

#endif