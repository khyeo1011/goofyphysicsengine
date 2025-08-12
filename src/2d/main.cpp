#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "util/util.h"
#include "object.h"
#include "rectangle.h"

int main()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "Goofy Physics Engine", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL(glfwGetProcAddress);
  glViewport(0, 0, 800, 800);

  Shader shaderProgram("default.vert", "default.frag");

  VAO VAO1;

  bufferObjects *polygons = new bufferObjects(VAO1, 2048, 2048);
  VAO1.Unbind();
  Rectangle rect0({0.0, 0.0}, 0.25, 0.25);
  Rectangle rect1{{0.5, 0.5}, 0.25, 0.5};
  while (!glfwWindowShouldClose(window))
  {
    flushBuffer(polygons);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.Activate();
    VAO1.Bind();
    rect0.draw(polygons);
    rect1.draw(polygons);
    glDrawElements(GL_TRIANGLES, *polygons->ebo.curr, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
    flushBuffer(polygons);
  }

  VAO1.Delete();
  shaderProgram.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}