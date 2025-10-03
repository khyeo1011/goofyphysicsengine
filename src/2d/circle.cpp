#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include <cmath>

Circle::Circle(point center, GLfloat radius, RGBAColor* color, GLfloat dx, GLfloat dy) {
    this->center = center;
    this->radius = radius;
    this->color = color;
    this->dx = dx;
    this->dy = dy;
    this->dtheta = 0; // Circles don't rotate
    this->mass = 10;
    this->torqueElasticity = 0;
}

void Circle::draw(bufferObjects* buffers) {
    // Simple triangle fan for circle
    const int num_segments = 36;
    GLfloat p[NUMBER_COLUMNS_VBO * (num_segments + 1)];
    GLuint i[3 * num_segments];

    p[0] = center.first;
    p[1] = center.second;
    p[2] = 0.0f;
    p[3] = color->r;
    p[4] = color->g;
    p[5] = color->b;

    for (int j = 0; j < num_segments; ++j) {
        float angle = 2.0f * M_PI * float(j) / float(num_segments);
        p[(j + 1) * NUMBER_COLUMNS_VBO] = center.first + radius * cos(angle);
        p[(j + 1) * NUMBER_COLUMNS_VBO + 1] = center.second + radius * sin(angle);
        p[(j + 1) * NUMBER_COLUMNS_VBO + 2] = 0.0f;
        p[(j + 1) * NUMBER_COLUMNS_VBO + 3] = color->r;
        p[(j + 1) * NUMBER_COLUMNS_VBO + 4] = color->g;
        p[(j + 1) * NUMBER_COLUMNS_VBO + 5] = color->b;
    }

    for (int j = 0; j < num_segments; ++j) {
        i[j * 3] = buffers->vertex_index;
        i[j * 3 + 1] = buffers->vertex_index + j + 1;
        i[j * 3 + 2] = buffers->vertex_index + (j + 1) % num_segments + 1;
    }

    buffers->addIndices(&i[0], 3 * num_segments);
    buffers->addVertices(&p[0], NUMBER_COLUMNS_VBO * (num_segments + 1));
    buffers->vertex_index += (num_segments + 1);
}

void Circle::update(GLfloat deltaTime) {
    if ((center.first + dx * deltaTime - radius <= leftLimit) || (center.first + dx * deltaTime + radius >= rightLimit)) {
        dx = -dx * elasticity;
    }
    if ((center.second + dy * deltaTime - radius <= downLimit) || (center.second + dy * deltaTime + radius >= upLimit)) {
        dy = -dy * elasticity;
    }

    center.first += dx * deltaTime;
    center.second += dy * deltaTime;
}

bool Circle::isCollidingWith(const Object* other) const {
    return other->isCollidingWithCircle(this);
}

bool Circle::isCollidingWithRectangle(const Rectangle* rect) const {
    // TODO: Implement circle-rectangle collision
    return false;
}

bool Circle::isCollidingWithCircle(const Circle* circ) const {
    float distSq = (center.first - circ->center.first) * (center.first - circ->center.first) +
                   (center.second - circ->center.second) * (center.second - circ->center.second);
    float radiusSum = radius + circ->radius;
    return distSq <= radiusSum * radiusSum;
}

bool Circle::isCollidingWithTriangle(const Triangle *tri) const {
    // TODO: Implement
    return false;
}
