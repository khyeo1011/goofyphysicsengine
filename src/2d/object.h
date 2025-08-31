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

  virtual bool isCollidingWith(const Object *other) const = 0;
  virtual bool isCollidingWithRectangle(const class Rectangle *rect) const = 0;

  void applyGravity(float gravity){
    dy -= gravity;
  }

  static void defineLimits(int screenWidth, int screenHeight)
  {
    leftLimit = -(float)screenWidth / 2.0;
    rightLimit = (float)screenWidth / 2.0;
    upLimit = (float)screenHeight / 2.0;
    downLimit = -(float)screenHeight / 2.0;
  }

  // virtual void applyForce(point location, point force);

  // virtual void applyTorque(GLfloat torque);

  // virtual bool isInObject(point coordinate);
  // Center of the object
  point center;
  // Angular Velocity
  float dtheta;
  // Angle of the object
  float theta;
  // Velocity of object x-axis
  float dx;
  // Velocity of object y-axis
  float dy;
  // Color of the object
  RGBAColor *color;
  // Mass of obect
  float mass = 10;
  // Ratio of the force applied that goes to torque
  float torqueElasticity = 1;

protected:
};

#endif