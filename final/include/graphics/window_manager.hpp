#ifndef WINDOW_MANAGER_HPP_
#define WINDOW_MANAGER_HPP_

#include "../config.hpp"
#include <GLFW/glfw3.h>

// Window manager namespace
namespace WM
{

void open_window();
bool is_window_open();
GLFWwindow *get_window();
void close_window();

}

#endif // WINDOW_MANAGER_HPP_
