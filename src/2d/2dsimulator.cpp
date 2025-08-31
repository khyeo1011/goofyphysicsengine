#include "2dsimulator.h"
#include "object.h"
#include "rectangle.h"
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <utility>
namespace
{
  // Helper for collision response
  point computeCollisionNormal(const Rectangle *rectA, const Rectangle *rectB)
  {
    float dx = rectB->getCenter().first - rectA->getCenter().first;
    float dy = rectB->getCenter().second - rectA->getCenter().second;
    float len = std::sqrt(dx * dx + dy * dy);
    if (len == 0)
      return {1, 0};
    return {dx / len, dy / len};
  }
}

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
  float cellSize = 100.5f; // Adjust based on your largest object
  std::unordered_map<std::pair<int, int>, std::vector<Object *>, CellHash> grid;

  // Place objects in grid cells
  for (auto *obj : objects)
  {
    auto *rect = dynamic_cast<Rectangle *>(obj);
    if (!rect)
      continue;

    // Compute bounding box
    float minX = rect->getMinX();
    float maxX = rect->getMaxX();
    float minY = rect->getMinY();
    float maxY = rect->getMaxY();

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
                // --- Your collision response here (as before) ---
                Rectangle *rectA = dynamic_cast<Rectangle *>(cellObjs[i]);
                Rectangle *rectB = dynamic_cast<Rectangle *>(neighborObjs[j]);
                if (!rectA || !rectB)
                  continue;

                point normal = computeCollisionNormal(rectA, rectB);

                float relVelX = rectB->dx - rectA->dx;
                float relVelY = rectB->dy - rectA->dy;
                float relVelAlongNormal = relVelX * normal.first + relVelY * normal.second;
                if (relVelAlongNormal > 0)
                  continue;

                float mA = rectA->mass;
                float mB = rectB->mass;
                float e = Object::elasticity;

                float impulse = -(1 + e) * relVelAlongNormal / (1.0f / mA + 1.0f / mB);
                float impulseX = impulse * normal.first;
                float impulseY = impulse * normal.second;

                rectA->dx -= impulseX / mA;
                rectA->dy -= impulseY / mA;
                rectB->dx += impulseX / mB;
                rectB->dy += impulseY / mB;

                float contactX = (rectA->getCenter().first + rectB->getCenter().first) / 2.0f;
                float contactY = (rectA->getCenter().second + rectB->getCenter().second) / 2.0f;
                float rAx = contactX - rectA->getCenter().first;
                float rAy = contactY - rectA->getCenter().second;
                float rBx = contactX - rectB->getCenter().first;
                float rBy = contactY - rectB->getCenter().second;
                float torqueA = rAx * impulseY - rAy * impulseX;
                float torqueB = rBx * (-impulseY) - rBy * (-impulseX);

                float IA = rectA->getMomentOfInertia();
                float IB = rectB->getMomentOfInertia();

                rectA->dtheta += rectA->torqueElasticity * torqueA / IA;
                rectB->dtheta += rectB->torqueElasticity * torqueB / IB;
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
