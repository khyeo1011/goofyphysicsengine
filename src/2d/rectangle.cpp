#include "rectangle.h"

void Rectangle::updateVertices()
{
  vertices[0] = {(-length / 2), (width / 2)};  // top-left
  vertices[1] = {(length / 2), (width / 2)};   // top-right
  vertices[2] = {(length / 2), (-width / 2)};  // low-right
  vertices[3] = {(-length / 2), (-width / 2)}; // low-left

  for (int j = 0; j < 4; j++)
  {
    rotatePoint(vertices[j], theta);
    vertices[j].first += center.first;
    vertices[j].second += center.second;
  }
}

void Rectangle::draw(bufferObjects *buffers)
{
  // std::cout << "Entering draw func: Rect" << std::endl;
  GLuint i[NUMBER_VERTICES_TRIANGLE * NUM_TRIANGLE_RECT];
  GLfloat p[NUMBER_COLUMNS_VBO * NUM_VERTIX_RECT];

  for (int j = 0; j < 4; j++)
  {
    p[j * NUMBER_COLUMNS_VBO] = vertices[j].first;
    p[j * NUMBER_COLUMNS_VBO + 1] = vertices[j].second;
    p[j * NUMBER_COLUMNS_VBO + 2] = 0.0;
    p[j * NUMBER_COLUMNS_VBO + 3] = color.r;
    p[j * NUMBER_COLUMNS_VBO + 4] = color.g;
    p[j * NUMBER_COLUMNS_VBO + 5] = color.b;
  }
  // Indices for the triangles

  i[0] = buffers->vertex_index + 0;
  i[1] = buffers->vertex_index + 2;
  i[2] = buffers->vertex_index + 1;
  i[3] = buffers->vertex_index + 0;
  i[4] = buffers->vertex_index + 3;
  i[5] = buffers->vertex_index + 2;

  buffers->addIndices(&i[0], NUM_TRIANGLE_RECT * NUMBER_VERTICES_TRIANGLE);
  buffers->addVertices(&p[0], NUMBER_COLUMNS_VBO * NUM_VERTIX_RECT);
  buffers->vertex_index += NUM_VERTIX_RECT;
  // std::cout << "Exiting draw func: Rect" << std::endl;
}

void Rectangle::update(GLfloat deltaTime)
{
  bool changeX = true, changeY = true;
  for (int i = 0; i < NUM_VERTIX_RECT; i++)
  {
    if (changeX && ((dx * deltaTime + vertices[i].first <= leftLimit) || (dx * deltaTime + vertices[i].first >= rightLimit)))
    {
      dx = -dx * elasticity;
      changeX = false;
    }
    if (changeY && ((dy * deltaTime + vertices[i].second <= downLimit) || (dy * deltaTime + vertices[i].second >= upLimit)))
    {
      dy = -dy * elasticity;
      changeY = false;
    }
  }
  center.first += dx * deltaTime;
  center.second += dy * deltaTime;
  theta = fmod((theta + dtheta * deltaTime), M_PI);
  updateVertices();
}