#include "util.h"

std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return (contents);
  }
  throw(errno);
}

Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
  // Read vertexFile and fragmentFile and store the strings
  std::string vertexCode = get_file_contents(vertexFile);
  std::string fragmentCode = get_file_contents(fragmentFile);

  // Convert the shader source strings into character arrays
  const char *vertexSource = vertexCode.c_str();
  const char *fragmentSource = fragmentCode.c_str();

  // Create Vertex Shader Object and get its reference
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // Attach Vertex Shader source to the Vertex Shader Object
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  // Compile the Vertex Shader into machine code
  glCompileShader(vertexShader);

  // Create Fragment Shader Object and get its reference
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // Attach Fragment Shader source to the Fragment Shader Object
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  // Compile the Vertex Shader into machine code
  glCompileShader(fragmentShader);

  // Create Shader Program Object and get its reference
  ID = glCreateProgram();
  // Attach the Vertex and Fragment Shaders to the Shader Program
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  // Wrap-up/Link all the shaders together into the Shader Program
  glLinkProgram(ID);

  // Delete the now useless Vertex and Fragment Shader objects
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
  glUseProgram(ID);
}

void Shader::Delete()
{
  glDeleteProgram(ID);
}

void VBO::Bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
  glDeleteBuffers(1, &ID);
}

VAO::VAO()
{
  glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset)
{
  VBO.Bind();
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  VBO.Unbind();
}

void VAO::Bind()
{
  glBindVertexArray(ID);
}

void VAO::Unbind()
{
  glBindVertexArray(0);
}

void VAO::Delete()
{
  glDeleteVertexArrays(1, &ID);
}

void EBO::Bind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
  glDeleteBuffers(1, &ID);
}

void flushBuffer(bufferObjects *buffers)
{
  *buffers->ebo->curr = 0;
  *buffers->vbo->curr = 0;
  buffers->vertex_index = 0;
}


void rotatePoint(point &p, GLfloat theta){
  GLfloat x = p.first;
  GLfloat y = p.second;
  GLfloat sint = sin(theta);
  GLfloat cost = cos(theta);
  p.first = x * cost - y * sint;
  p.second = x * sint + y * cost;
}