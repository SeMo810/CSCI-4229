#include <GLFW/glfw3.h>
#include <log.hpp>


void normalexit()
{
  LOG::warn("Logger is not closed normally during a normal exit.");
  LOG::close();
}

void terminateexit()
{
  LOG::error("The program exited in a non-standard fashion.");
  LOG::close();
}

int main(int argc, char **argv)
{
  atexit(normalexit);
  std::set_terminate(terminateexit);

  if (!LOG::open())
    fprintf(stderr, "Could not open the logger. Logging will be unavailable.\n");

  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(600, 400, "CSCI 4229 Final - Sean Moss", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  LOG::close();
  return 0;
}
