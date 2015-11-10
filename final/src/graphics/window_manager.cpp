#include "graphics/window_manager.hpp"

namespace WM
{

static GLFWwindow *g_window = nullptr;

void open_window()
{

}

bool is_window_open()
{
  return (g_window != nullptr) && !glfwWindowShouldClose(window);
}

GLFWwindow *get_window()
{

}

void close_window()
{

}

}
