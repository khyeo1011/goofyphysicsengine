#include <iostream>
#include <math.h>
#include "util/util.h"
#include "object.h"
#include "rectangle.h"
static unsigned int target_fps = 60;
static double frame_limit = 1.0 / 60;

int main()
{
  glfwInit();
  target_fps = 60;
  frame_limit = 1.0 / target_fps;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "Goofy Physics Engine 2d", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL(glfwGetProcAddress);
  glViewport(0, 0, 800, 800);

  Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

  VAO VAO1;

  bufferObjects *polygons = new bufferObjects(VAO1, 2048, 2048);
  VAO1.Unbind();

  Rectangle rect0({0.0, 0.0}, 0.25, 0.25);
  Rectangle rect1({0.5, 0.5}, 0.2, 0.4, {1, 1, 1, 1}, 45, 0, 0, 1);
  /* // For Debugging
    VAO1.Bind();
    rect0.draw(polygons);
    rect1.draw(polygons);

    GLfloat *flptr = (GLfloat *)glMapNamedBuffer(polygons->vbo->ID, GL_READ_ONLY);
    GLuint *inptr = (GLuint *)glMapNamedBuffer(polygons->ebo->ID, GL_READ_ONLY);
    std::cout << "Vertex buffer contents:" << std::endl;
    for (size_t i = 0; i < *polygons->vbo->curr; ++i)
    {
      std::cout << flptr[i] << " ";
      // Print newline every 2 or 3 floats if your vertex format is vec2 or vec3
      if ((i + 1) % 6 == 0)
        std::cout << std::endl;
    }
    std::cout << "\nIndex buffer contents:" << std::endl;
    for (size_t i = 0; i < *polygons->ebo->curr; ++i)
    {
      std::cout << inptr[i] << " ";
      if ((i + 1) % 3 == 0)
        std::cout << std::endl;
    } */
  double last_time = glfwGetTime();
  double last_update_time = glfwGetTime();
  double accumulated_time = 0.0;
  double current_time = glfwGetTime();
  double update_dt;
  double dt;
  while (!glfwWindowShouldClose(window))
  {
    // Updates counter and times
    current_time = glfwGetTime();
    dt = current_time - last_time;
    last_time = current_time;
    accumulated_time += dt;

    glfwPollEvents();
    if (accumulated_time >= frame_limit)
    {
      update_dt = current_time - last_update_time;
      last_update_time = current_time;
      std::string FPS = std::to_string(1 / accumulated_time);
      std::string ms = std::to_string((dt) * 1000);
      std::string newTitle = "YoutubeOpenGL - " + FPS + "FPS / " + ms + "ms";
      rect1.update(update_dt);
      glfwSetWindowTitle(window, newTitle.c_str());
      accumulated_time = 0.0;
      glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      shaderProgram.Activate();
      VAO1.Bind();
      rect0.draw(polygons);
      rect1.draw(polygons);
      glDrawElements(GL_TRIANGLES, *polygons->ebo->curr, GL_UNSIGNED_INT, 0);
      glfwSwapBuffers(window);
      flushBuffer(polygons);
    }
  }

  VAO1.Delete();
  shaderProgram.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}