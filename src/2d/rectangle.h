#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H
#include "object.h"
#define NUM_VERTIX_RECT   4
#define NUM_TRIANGLE_RECT 2

class Rectangle : public Object
{
public:
  Rectangle(point center = {0, 0}, GLfloat length = 1, GLfloat width = 1, RGBAColor color = {1.0, 1.0, 1.0, 1.0}, GLfloat angle = 0, GLfloat dx = 0, GLfloat dy = 0, GLfloat dtheta = 0)
  {
    this->center = center;
    this->length = length;
    this->width = width;
    this->theta = angle;
    this->dtheta = dtheta;
    this->dx = dx;
    this->dy = dy;
    this->color.r = color.r;
    this->color.r = color.g;
    this->color.r = color.b;
    this->color.r = color.a;
    updateVertices();
  };


  
  //Add the vertices of the objects into the VBO
  void draw(bufferObjects* buffers) override;


  void update(GLfloat deltatime) override;

  void updateVertices();
private:
  GLfloat length;
  GLfloat width;
  point vertices[4];
};

#endif