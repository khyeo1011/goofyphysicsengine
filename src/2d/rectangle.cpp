#include "rectangle.h"

void Rectangle::update(GLfloat deltatime)
{
}

void Rectangle::draw(bufferObjects *buffers)
{
  // std::cout << "Entering draw func: Rect" << std::endl;
  GLuint i[3 * NUM_TRIANGLE_RECT];
  GLfloat p[3 * NUM_VERTIX_RECT];

  point vertices[4];
  vertices[0] = {(center.first - length / 2), (center.second + width / 2)}; // top-left
  vertices[1] = {(center.first + length / 2), (center.second + width / 2)}; // top-right
  vertices[2] = {(center.first + length / 2), (center.second - width / 2)}; // low-right
  vertices[3] = {(center.first - length / 2), (center.second - width / 2)}; // low-left

  for (int j = 0; j < 4; j++)
  {
    p[j * 3] = vertices[j].first;
    p[j * 3 + 1] = vertices[j].second;
    p[j * 3 + 2] = 0.0;
  }
  // Indices for the triangles

  i[0] = *buffers->ebo.curr + 0;
  i[1] = *buffers->ebo.curr + 2;
  i[2] = *buffers->ebo.curr + 1;
  i[3] = *buffers->ebo.curr + 0;
  i[4] = *buffers->ebo.curr + 3;
  i[5] = *buffers->ebo.curr + 2;

  buffers->addIndices(&i[0], 3 * NUM_TRIANGLE_RECT);
  buffers->addVertices(&p[0], 3 * NUM_VERTIX_RECT);
  // std::cout << "Exiting draw func: Rect" << std::endl;
}
