#include "2dsimulator.h"
#include "object.h"
#include "rectangle.h"
#include "circle.h"
#include "triangle.h"
#include "collision.h"
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <utility>

void Simulator2D::update(double dt)
{
  for (auto &obj : objects)
  {
    obj->update(dt);
    obj->applyGravity(dt * fG);
  }
  handleCollisions();
}

struct CellHash
{
  std::size_t operator()(const std::pair<int, int> &cell) const
  {
    return std::hash<int>()(cell.first) ^ (std::hash<int>()(cell.second) << 1);
  }
};

void Simulator2D::handleCollisions()
{
  float cellSize = 200.0f; // Adjust based on your largest object
  std::unordered_map<std::pair<int, int>, std::vector<Object *>, CellHash> grid;

  // Place objects in grid cells
  for (auto *obj : objects)
  {
    float minX, maxX, minY, maxY;
    if (auto *rect = dynamic_cast<Rectangle *>(obj)) {
        minX = rect->getMinX();
        maxX = rect->getMaxX();
        minY = rect->getMinY();
        maxY = rect->getMaxY();
    } else if (auto *circ = dynamic_cast<Circle *>(obj)) {
        minX = circ->center.first - circ->getRadius();
        maxX = circ->center.first + circ->getRadius();
        minY = circ->center.second - circ->getRadius();
        maxY = circ->center.second + circ->getRadius();
    } else {
        continue;
    }

    int cellMinX = static_cast<int>(std::floor(minX / cellSize));
    int cellMaxX = static_cast<int>(std::floor(maxX / cellSize));
    int cellMinY = static_cast<int>(std::floor(minY / cellSize));
    int cellMaxY = static_cast<int>(std::floor(maxY / cellSize));

    for (int cx = cellMinX; cx <= cellMaxX; ++cx)
    {
      for (int cy = cellMinY; cy <= cellMaxY; ++cy)
      {
        grid[{cx, cy}].push_back(obj);
      }
    }
  }

  // Check collisions within each cell and neighboring cells
  for (const auto &[cell, cellObjs] : grid)
  {
    int cx = cell.first, cy = cell.second;
    for (int dx = -1; dx <= 1; ++dx)
    {
      for (int dy = -1; dy <= 1; ++dy)
      {
        auto neighborCell = std::make_pair(cx + dx, cy + dy);
        if (!grid.count(neighborCell))
          continue;
        const auto &neighborObjs = grid[neighborCell];
        for (size_t i = 0; i < cellObjs.size(); ++i)
        {
          for (size_t j = 0; j < neighborObjs.size(); ++j)
          {
            // Avoid duplicate checks and self-collision
            if (cellObjs[i] == neighborObjs[j])
              continue;
            // To avoid double response, only handle if cellObjs[i] < neighborObjs[j] by pointer
            if (cellObjs[i] < neighborObjs[j])
            {
              if (cellObjs[i]->isCollidingWith(neighborObjs[j]))
              {
                Collision::handleCollision(cellObjs[i], neighborObjs[j]);
              }
            }
          }
        }
      }
    }
  }
}

void Simulator2D::drawAll(bufferObjects *buffers)
{
  for (auto &obj : objects)
  {
    obj->draw(buffers);
  }
}

Simulator2D::~Simulator2D()
{
  for (auto &obj : objects)
  {
    delete obj;
  }
}
