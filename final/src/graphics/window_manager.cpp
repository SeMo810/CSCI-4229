#include "graphics/window_manager.hpp"
#include "log.hpp"

int ERR::GLFW_ERROR_CODE = 0;
String ERR::GLFW_ERROR_STRING = "";

void glfw_error_callback(int code, const char *err)
{
  ERR::GLFW_ERROR_CODE = code;
  ERR::GLFW_ERROR_STRING = String(err);
}

namespace WM
{

static GLFWwindow *g_window = nullptr;
static float g_deltaTime = 0.0f;

bool open_window()
{
  if (g_window)
  {
    LOG::warn("Cannot open the window twice.");
    return true;
  }

  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit())
  {
    LOG::error("Could not initialize GLFW! Error: (%d)=>\"%s\".", ERR::GLFW_ERROR_CODE, ERR::GLFW_ERROR_STRING.c_str());
    return false;
  }
  LOG::info("GLFW was successfully initialized.");

  // TODO: DETECT THE HIGHEST AVAILABLE RESOLUTION, AND DEFAULT TO THAT
  int width = 1600, height = 900;

  glfwWindowHint(GLFW_RESIZABLE, 0);
  g_window = glfwCreateWindow(width, height, "CSCI 4229 Final - Sean Moss", nullptr, nullptr);
  if (!g_window)
  {
    LOG::error("Could not open GLFW window! Error: (%d)=>\"%s\".", ERR::GLFW_ERROR_CODE, ERR::GLFW_ERROR_STRING.c_str());
    glfwTerminate();
    return false;
  }

  glfwGetWindowSize(g_window, &width, &height);
  LOG::info("Opened a window with the size %dx%d.", width, height);
  return true;
}

bool is_window_open()
{
  return (g_window != nullptr) && !glfwWindowShouldClose(g_window);
}

GLFWwindow *get_window()
{
  return g_window;
}

void ensure_context()
{
  static bool warned = false;
  if (!g_window)
  {
    if (!warned)
      LOG::warn("Attempted to ensure a context before the window was created. Note, further messages about"
        " this error will be suppressed, until a valid context is set again.");
    warned = true;
  }

  glfwMakeContextCurrent(g_window);
  warned = false;
}

void clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void do_frame()
{
  static bool warned = false;
  if (!g_window)
  {
    if (!warned)
      LOG::warn("Attempted to run a frame before the window was created. Note, further messages about"
        " this error will be suppressed, until a valid context is set again.");
    warned = true;
  }

  static float lasttime = 0.0f;
  float ntime = (float)glfwGetTime();
  g_deltaTime = (ntime - lasttime);
  lasttime = ntime;

  glfwSwapBuffers(g_window);
  glfwPollEvents();
  warned = false;
}

void close_window()
{
  glfwSetWindowShouldClose(g_window, 1);
}

void terminate()
{
  if (!g_window)
  {
    LOG::warn("Cannot terminate GLFW if there was no active GLFW instance.");
    return;
  }

  glfwDestroyWindow(g_window);
  g_window = nullptr;
  glfwTerminate();

  glfwSetErrorCallback(nullptr);
  LOG::info("GLFW was shut down.");
}

float get_frame_time()
{
  return g_deltaTime;
}

}
