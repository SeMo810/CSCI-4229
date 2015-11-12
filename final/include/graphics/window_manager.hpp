#ifndef WINDOW_MANAGER_HPP_
#define WINDOW_MANAGER_HPP_

#include "../config.hpp"
#include <GLFW/glfw3.h>

// Window manager namespace
namespace WM
{

extern bool open_window();
extern bool is_window_open();
extern GLFWwindow *get_window();
extern void ensure_context();
extern void do_frame();
extern void close_window();
extern void terminate();

extern float get_frame_time();

}

#endif // WINDOW_MANAGER_HPP_
