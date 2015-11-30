#include <GLFW/glfw3.h>
#include "log.hpp"
#include "input.hpp"
#include "graphics/camera.hpp"
#include "graphics/debug.hpp"
#include "graphics/window_manager.hpp"
#include "world/world.hpp"

void cleanup(bool crash)
{
  WORLD::destroy();
  WM::terminate();
}

void normalexit()
{
  cleanup(false);

  LOG::warn("Logger is not closed normally during a normal exit.");
  LOG::close();
}

void terminateexit()
{
  cleanup(true);

  LOG::error("The program exited in a non-standard fashion.");
  LOG::close();
}

int main(int argc, char **argv)
{
  atexit(normalexit);
  std::set_terminate(terminateexit);

  if (!LOG::open())
    fprintf(stderr, "Could not open the logger file. Logging to file will be unavailable.\n");

  if (!WM::open_window())
  {
    LOG::error("Could not initalize graphics system. Exiting...");
    return -1;
  }
  WM::ensure_context();
  INPUT::initialize();

  if (!WORLD::create())
  {
    LOG::error("Could not create the world. Exiting...");
    return -1;
  }

  while (WM::is_window_open())
  {
    float ftime = WM::get_frame_time();
    CAMERA::apply_camera_projection(true);

    WORLD::update(ftime);

    WM::clear();
    CAMERA::apply_camera_transforms();
    DEBUG::render_world_axes();
    WORLD::render();

    WM::do_frame();
  }

  cleanup(false);
  LOG::close();
  return 0;
}
