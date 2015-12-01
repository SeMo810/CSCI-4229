#include "input.hpp"
#include "log.hpp"
#include "graphics/lighting.hpp"
#include "graphics/ogl.hpp"
#include "graphics/window_manager.hpp"
#include "graphics/camera.hpp"

namespace INPUT
{

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
    switch (key)
    {
      case GLFW_KEY_A:
        CAMERA::move_camera(math::Vec2f(-0.2f, 0.0f));
        break;
      case GLFW_KEY_D:
        CAMERA::move_camera(math::Vec2f(0.2f, 0.0f));
        break;
      case GLFW_KEY_S:
        CAMERA::move_camera(math::Vec2f(0.0f, 0.2f));
        break;
      case GLFW_KEY_W:
        CAMERA::move_camera(math::Vec2f(0.0f, -0.2f));
        break;
      case GLFW_KEY_PERIOD:
        LIGHT::set_ambient_lighting(LIGHT::get_ambient_lighting() + 0.05);
        LIGHT::update_lighting();
        break;
      case GLFW_KEY_COMMA:
        LIGHT::set_ambient_lighting(LIGHT::get_ambient_lighting() - 0.05);
        LIGHT::update_lighting();
        break;
      case GLFW_KEY_APOSTROPHE:
        LIGHT::set_diffuse_lighting(LIGHT::get_diffuse_lighting() + 0.05);
        LIGHT::update_lighting();
        break;
      case GLFW_KEY_SEMICOLON:
        LIGHT::set_diffuse_lighting(LIGHT::get_diffuse_lighting() - 0.05);
        LIGHT::update_lighting();
        break;
      case GLFW_KEY_RIGHT_BRACKET:
        LIGHT::progress_time_of_day(-0.05);
        LIGHT::update_lighting();
        break;
      case GLFW_KEY_LEFT_BRACKET:
        LIGHT::progress_time_of_day(0.05);
        LIGHT::update_lighting();
        break;
      default:
        break;
    }
  }
  else if (action == GLFW_REPEAT)
  {
    switch (key)
    {
      case GLFW_KEY_A:
        CAMERA::move_camera(math::Vec2f(-0.2f, 0.0f));
        break;
      case GLFW_KEY_D:
        CAMERA::move_camera(math::Vec2f(0.2f, 0.0f));
        break;
      case GLFW_KEY_S:
        CAMERA::move_camera(math::Vec2f(0.0f, 0.2f));
        break;
      case GLFW_KEY_W:
        CAMERA::move_camera(math::Vec2f(0.0f, -0.2f));
        break;
      default:
        break;
    }
  }
  else if (action == GLFW_RELEASE)
  {
    switch (key)
    {
      default:
        break;
    }
  }
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{

}

static void mouse_scroll_callback(GLFWwindow *window, double xoff, double yoff)
{
  CAMERA::move_camera_distance(-yoff * 0.15);
}

void initialize()
{
  GLFWwindow *window = WM::get_window();
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, mouse_scroll_callback);
}

}
