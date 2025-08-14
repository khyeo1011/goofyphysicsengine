#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H
#include "object.h"
#define NUM_VERTIX_RECT   4
#define NUM_TRIANGLE_RECT 2

class Rectangle : public Object
{
public:
  Rectangle(point center = {0, 0}, GLfloat length = 1, GLfloat width = 1, RGBAColor* color = new RGBAColor(1,1,1,1), GLfloat angle = 0, GLfloat dx = 0, GLfloat dy = 0, GLfloat dtheta = 0)
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
    for(int i = 0; i < NUM_VERTIX_RECT; i++){
      if ((vertices[i].first <= leftLimit) || (vertices[i].first >= rightLimit) || (vertices[i].second <= downLimit) || (vertices[i].second >= upLimit))
      {
        center.first = 0;
        center.second = 0;
        length = 1;
        width = 1;
      }
    }
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