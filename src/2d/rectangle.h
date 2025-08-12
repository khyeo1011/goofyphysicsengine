#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H
#include "object.h"
const static int NUM_VERTIX_RECT = 4;
const static int NUM_TRIANGLE_RECT = 2;

class Rectangle : public Object
{
public:
  Rectangle(point center = {0, 0}, GLfloat length = 0, GLfloat width = 0, GLfloat angle = 0, GLfloat dx = 0, GLfloat dy = 0, GLfloat dtheta = 0)
  {
    this->center = center;
    this->length = length;
    this->width = width;
    this->theta = angle;
    this->dtheta = dtheta;
    this->dx = dx;
    this->dy = dy;
  };

  //Update coordinates of the object
  void update(GLfloat deltaTime) override;
  
  //Add the vertices of the objects into the VBO
  void draw(bufferObjects* buffers) override;
private:
  GLfloat length;
  GLfloat width;
};

#endif