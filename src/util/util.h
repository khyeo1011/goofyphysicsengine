#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>
#include <queue>
#include <math.h>




/*
 * Utility header for OpenGL and GLFW rendering.
 *
 * Contains:
 * - renderUtil struct: Aggregates window and OpenGL objects.
 * - get_file_contents: Loads file contents into a string.
 * - Shader class: Handles OpenGL shader program creation and management.
 * - VBO class: Manages Vertex Buffer Objects for vertex data.
 * - VAO class: Manages Vertex Array Objects for vertex attribute configuration.
 */

#define NUMBER_COLUMNS_VBO 6
#define NUMBER_VERTICES_TRIANGLE 3
typedef std::pair<GLfloat, GLfloat> point;


/**
 * @brief Loads the entire contents of a file into a std::string.
 * @param filename Path to the file to be loaded.
 * @return Contents of the file as a string.
 */
std::string get_file_contents(const char *filename);




struct RGBAColor{
  GLfloat r;
  GLfloat g;
  GLfloat b;
  GLfloat a;
  RGBAColor(GLfloat r = 1,GLfloat g = 1, GLfloat b = 1, GLfloat a = 1){
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }
};



/**
 * @class Shader
 * @brief Encapsulates an OpenGL shader program, providing methods for creation, activation, and deletion.
 */
class Shader
{
public:
  GLuint ID; ///< OpenGL shader program ID.

  /**
   * @brief Constructs a Shader object by compiling and linking vertex and fragment shaders.
   * @param vertexFile Path to the vertex shader source file.
   * @param fragmentFile Path to the fragment shader source file.
   */
  Shader(const char *vertexFile, const char *fragmentFile);

  /**
   * @brief Activates the shader program for rendering.
   */
  void Activate();

  /**
   * @brief Deletes the shader program from OpenGL.
   */
  void Delete();
};

/**
 * @class VBO
 * @brief Manages an OpenGL Vertex Buffer Object for storing vertex data.
 */
class VBO
{
public:
  GLuint ID; ///< OpenGL VBO ID.
  GLintptr *curr;

  /**
   * @brief Creates a VBO and uploads vertex data to the GPU.
   * @param vertices Pointer to the vertex data array.
   * @param size Size of the vertex data in bytes.
   */
  VBO(GLsizeiptr size = 2048)
  {
    curr = new GLintptr;
    *curr = 0;
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
  }

  // Copy constructor
  VBO(const VBO &other)
  {
    curr = other.curr;
    ID = other.ID;
  }

  // Copy assignment operator
  VBO &operator=(const VBO &other)
  {
    if (this != &other)
    {
      curr = other.curr;
      ID = other.ID;
    }
    return *this;
  }

  /**
   * @brief Binds the VBO for use.
   */
  void Bind();

  /**
   * @brief Unbinds the VBO.
   */
  void Unbind();

  /**
   * @brief Deletes the VBO from OpenGL.
   */
  void Delete();
};

/**
 * @class VAO
 * @brief Manages an OpenGL Vertex Array Object for configuring vertex attribute pointers.
 */
class VAO
{
public:
  GLuint ID; ///< OpenGL VAO ID.

  /**
   * @brief Constructs a VAO object and generates the VAO in OpenGL.
   */
  VAO();

  /**
   * @brief Links a VBO to the VAO with a specified attribute layout.
   * @param VBO Reference to the VBO to link.
   * @param layout Attribute layout index.
   */
  void LinkAttrib(VBO& VBO, GLuint layout = 0, GLuint numComponents = 3, GLenum type = GL_FLOAT, GLsizeiptr stride = 6*sizeof(GLfloat), void* offset = nullptr);

  /**
   * @brief Binds the VAO for use.
   */
  void Bind();

  /**
   * @brief Unbinds the VAO.
   */
  void Unbind();

  /**
   * @brief Deletes the VAO from OpenGL.
   */
  void Delete();
};

class EBO
{
public:
  GLuint ID;
  GLintptr *curr;
  EBO(GLsizeiptr size = 2048)
  {
    curr = new GLintptr;
    *curr = 0;
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
  }

  // Copy constructor
  EBO(const EBO &other)
  {
    curr = other.curr;
    ID = other.ID;
  }

  // Copy assignment operator
  EBO &operator=(const EBO &other)
  {
    if (this != &other)
    {
      curr = other.curr;
      ID = other.ID;
    }
    return *this;
  }

  void Bind();
  void Unbind();
  void Delete();
};

/**
 * @struct renderUtil
 * @brief Holds pointers and objects required for storing vertices
 */
struct bufferObjects
{
  VAO *vao;
  VBO *vbo;
  EBO *ebo;
  unsigned int vertex_index = 0;
  bufferObjects(VAO &va, VBO &v, EBO &e, bool x)
  {
    vao = &va;
    vao->Bind();
    vbo = &v;
    vao->LinkAttrib(*vbo, 0);
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    ebo = &e;
  };

  bufferObjects(VAO &va, GLuint sizev = 2048, GLuint sizee = 2048)
  {
    vao = &va;
    vao->Bind();
    vbo = new VBO(sizev);
    vao->LinkAttrib(*vbo, 0);
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    ebo = new EBO(sizee);
  }

  void addVertices(GLfloat *val, GLsizeiptr size)
  {
    vbo->Bind();
    glNamedBufferSubData(vbo->ID, *vbo->curr * sizeof(GLfloat), size * sizeof(GLfloat), val);
    *vbo->curr += size;
  }
  void addIndices(GLuint *val, GLsizeiptr size)
  {
    ebo->Bind();
    glNamedBufferSubData(ebo->ID, *ebo->curr * sizeof(GLuint), size * sizeof(GLuint), val);
    *ebo->curr += size;
  }
};



/**
 * Unbinds buffers and then draws the vertices and triangles based on the VBO
 * and EBO declarations, and then flushed the buffers to be reused.
 */
void flushBuffer(bufferObjects *buffers);

void rotatePoint(point &p, GLfloat theta);


#endif