#include <iostream>
#include <math.h>
#include "util/util.h"
#include "object.h"
#include "rectangle.h"
#include "2dsimulator.h"
static unsigned int target_fps = 60;
static double frame_limit = 1.0 / 60;

int main()
{
  target_fps = 60;
  frame_limit = 0.99 / target_fps;
  screenHeight = 900;
  screenWidth = 1600;
  heightScale = 2.0 / screenHeight;
  widthScale = 2.0 / screenWidth;
  Object::defineLimits(screenWidth, screenHeight);
  Object::elasticity = 0.99;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Goofy Physics Engine 2d", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL(glfwGetProcAddress);
  glViewport(0, 0, screenWidth, screenHeight);

  Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

  VAO VAO1;

  bufferObjects *polygons = new bufferObjects(VAO1, 204800, 204800);
  VAO1.Unbind();

  // Create multiple rectangles

  int numRects = 20;
  Simulator2D sim;
  srand((unsigned int)time(NULL));
  for (int i = 0; i < numRects; ++i)
  {
    GLfloat x = (rand() % (screenWidth - 200)) - (screenWidth / 2 - 100);
    GLfloat y = (rand() % (screenHeight - 200)) - (screenHeight / 2 - 100);
    GLfloat len = 10;
    GLfloat wid = 100;
    RGBAColor *color = new RGBAColor(
        (rand() % 501) / 1000.0f + 0.5,
        (rand() % 501) / 1000.0f + 0.5,
        (rand() % 501) / 1000.0f + 0.5,
        1.0f);
    GLfloat angle = ((rand() % 360) / 180.0f) * M_PI;
    GLfloat dx = (rand() % 2) - 1;
    GLfloat dy = (rand() % 100) - 1;
    GLfloat dtheta = ((rand() % 1001) / 1000.0f) * M_PI / 4.0f;
    sim.objects.push_back(new Rectangle({x, y}, len, wid, color, angle, dx, dy, dtheta));
    sim.objects[i]->mass = (rand() % 1000);
  }

  GLuint scaleXID = glGetUniformLocation(shaderProgram.ID, "scaleX");
  GLuint scaleYID = glGetUniformLocation(shaderProgram.ID, "scaleY");
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
      std::string newTitle = "Goofy Physics Engine 2d - " + FPS + "FPS / " + ms + "ms";
      sim.update(update_dt);
      glfwSetWindowTitle(window, newTitle.c_str());
      accumulated_time = 0.0;
      glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      shaderProgram.Activate();
      glUniform1f(scaleXID, widthScale);
      glUniform1f(scaleYID, heightScale);
      VAO1.Bind();
      sim.drawAll(polygons);
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