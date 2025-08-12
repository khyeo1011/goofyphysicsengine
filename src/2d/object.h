#ifndef OBJECT_H
#define OBJECT_H
#include "util/util.h"
#include <math.h>
typedef std::pair<GLfloat, GLfloat> point;

class Object
{
public:
  virtual ~Object() = default;
  //Update coordinates of the object
  virtual void update(GLfloat deltaTime) = 0;
  
  //Add the vertices of the objects into the VBO
  virtual void draw(bufferObjects* buffers) = 0;

protected:
  point center;
  GLfloat dtheta;
  GLfloat theta;
  GLfloat dx;
  GLfloat dy;
};

#endif