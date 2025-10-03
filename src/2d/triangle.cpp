#include "triangle.h"
#include "rectangle.h"
#include "circle.h"

// Helper: Project vertices onto axis
static void projectOntoAxis(const point *verts, int n, const point &axis, float &min, float &max)
{
  min = max = (verts[0].first * axis.first + verts[0].second * axis.second);
  for (int i = 1; i < n; ++i)
  {
    float proj = verts[i].first * axis.first + verts[i].second * axis.second;
    if (proj < min)
      min = proj;
    if (proj > max)
      max = proj;
  }
}

// Helper: Get triangle axes (normals)
static std::vector<point> getAxes(const point *verts, int n)
{
  std::vector<point> axes;
  for (int i = 0; i < n; ++i)
  {
    int next = (i + 1) % n;
    float dx = verts[next].first - verts[i].first;
    float dy = verts[next].second - verts[i].second;
    // Normal (perpendicular)
    point axis = {-dy, dx};
    float len = std::sqrt(axis.first * axis.first + axis.second * axis.second);
    axis.first /= len;
    axis.second /= len;
    axes.push_back(axis);
  }
  return axes;
}

Triangle::Triangle(point v1, point v2, point v3, RGBAColor* color) {
    this->vertices[0] = v1;
    this->vertices[1] = v2;
    this->vertices[2] = v3;
    this->color = color;
}

void Triangle::draw(bufferObjects* buffers) {
    GLuint i[3];
    GLfloat p[NUMBER_COLUMNS_VBO * 3];

    for (int j = 0; j < 3; j++) {
        p[j * NUMBER_COLUMNS_VBO] = vertices[j].first;
        p[j * NUMBER_COLUMNS_VBO + 1] = vertices[j].second;
        p[j * NUMBER_COLUMNS_VBO + 2] = 0.0;
        p[j * NUMBER_COLUMNS_VBO + 3] = color->r;
        p[j * NUMBER_COLUMNS_VBO + 4] = color->g;
        p[j * NUMBER_COLUMNS_VBO + 5] = color->b;
    }

    i[0] = buffers->vertex_index + 0;
    i[1] = buffers->vertex_index + 1;
    i[2] = buffers->vertex_index + 2;

    buffers->addIndices(&i[0], 3);
    buffers->addVertices(&p[0], NUMBER_COLUMNS_VBO * 3);
    buffers->vertex_index += 3;
}

void Triangle::update(GLfloat deltaTime) {}

bool Triangle::isCollidingWith(const Object* other) const {
    return other->isCollidingWithTriangle(this);
}

bool Triangle::isCollidingWithRectangle(const Rectangle* rect) const {
    return rect->isCollidingWithTriangle(this);
}

bool Triangle::isCollidingWithCircle(const Circle* circ) const {
    auto axes = getAxes(this->vertices, 3);

    // Also check axis from circle center to closest triangle vertex
    point closestVertex = vertices[0];
    float minD_sq = pow(circ->center.first - vertices[0].first, 2) + pow(circ->center.second - vertices[0].second, 2);

    for(int i = 1; i < 3; ++i) {
        float d_sq = pow(circ->center.first - vertices[i].first, 2) + pow(circ->center.second - vertices[i].second, 2);
        if (d_sq < minD_sq) {
            minD_sq = d_sq;
            closestVertex = vertices[i];
        }
    }

    point axisToCircle = {closestVertex.first - circ->center.first, closestVertex.second - circ->center.second};
    float len = std::sqrt(axisToCircle.first * axisToCircle.first + axisToCircle.second * axisToCircle.second);
    if (len > 0) {
        axisToCircle.first /= len;
        axisToCircle.second /= len;
        axes.push_back(axisToCircle);
    }

    for (const auto& axis : axes) {
        float minA, maxA, minB, maxB;
        projectOntoAxis(this->vertices, 3, axis, minA, maxA);

        // Project circle onto axis
        float centerProj = circ->center.first * axis.first + circ->center.second * axis.second;
        minB = centerProj - circ->getRadius();
        maxB = centerProj + circ->getRadius();

        if (maxA < minB || maxB < minA) {
            return false;
        }
    }

    return true;
}

bool Triangle::isCollidingWithTriangle(const Triangle* tri) const {
    auto axesA = getAxes(this->vertices, 3);
    auto axesB = getAxes(tri->vertices, 3);

    for (const auto &axis : axesA) {
        float minA, maxA, minB, maxB;
        projectOntoAxis(this->vertices, 3, axis, minA, maxA);
        projectOntoAxis(tri->vertices, 3, axis, minB, maxB);
        if (maxA < minB || maxB < minA)
            return false;
    }

    for (const auto &axis : axesB) {
        float minA, maxA, minB, maxB;
        projectOntoAxis(this->vertices, 3, axis, minA, maxA);
        projectOntoAxis(tri->vertices, 3, axis, minB, maxB);
        if (maxA < minB || maxB < minA)
            return false;
    }

    return true;
}
