#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cerrno>
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

/**
 * @brief Loads the entire contents of a file into a std::string.
 * @param filename Path to the file to be loaded.
 * @return Contents of the file as a string.
 */
std::string get_file_contents(const char *filename);

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
  void LinkVBO(VBO &VBO, GLuint layout);

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
  VBO vbo;
  EBO ebo;
  bufferObjects(VAO &va, VBO &v, EBO &e, bool x)
  {
    vao = &va;
    vao->Bind();
    vbo = v;
    vao->LinkVBO(vbo, 0);
    ebo = e;
  };

  bufferObjects(VAO &va, GLuint sizev = 8196, GLuint sizee = 2048)
  {
    vao = &va;
    vao->Bind();
    vbo = VBO(2048);
    vao->LinkVBO(vbo, 0);
    ebo = EBO(2048);
  }

  void addVertices(GLfloat *val, GLsizeiptr size)
  {
    glNamedBufferSubData(vbo.ID, *vbo.curr * sizeof(GLfloat), size * sizeof(GLfloat), val);
    *vbo.curr += size;
  }
  void addIndices(GLuint *val, GLsizeiptr size)
  {
    glNamedBufferSubData(ebo.ID, *ebo.curr * sizeof(GLuint), size * sizeof(GLuint), val);
    *ebo.curr += size;
  }
};

/**
 * Unbinds buffers and then draws the vertices and triangles based on the VBO
 * and EBO declarations, and then flushed the buffers to be reused.
 */
void flushBuffer(bufferObjects *buffers);

#endif