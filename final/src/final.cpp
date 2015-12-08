#include <GLFW/glfw3.h>
#include "log.hpp"
#include "input.hpp"
#include "graphics/camera.hpp"
#include "graphics/debug.hpp"
#include "graphics/lighting.hpp"
#include "graphics/window_manager.hpp"
#include "world/ship.hpp"
#include "world/scripter.hpp"
#include "world/world.hpp"

void cleanup(bool crash)
{
  SCRIPT::end_script();
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
  LIGHT::initialize_lighting();
  // Temporary
  LIGHT::set_ambient_lighting(.2);
  LIGHT::set_diffuse_lighting(.5);
  LIGHT::update_lighting();
  INPUT::initialize();

  // Attempt to load the script
  if (!SCRIPT::load_script_file())
  {
    LOG::error("Could not load the script file. Exiting...");
    cleanup(true);
    LOG::close();
    return -1;
  }

  // Attempt to create the world
  if (!WORLD::create())
  {
    LOG::error("Could not create the world. Exiting...");
    cleanup(true);
    LOG::close();
    return -1;
  }

  SCRIPT::start_script();
  while (WM::is_window_open())
  {
    float ftime = WM::get_frame_time();
    CAMERA::apply_camera_projection(true);

    WORLD::update(ftime);

    WM::clear();
    CAMERA::apply_camera_transforms();
    WORLD::render();

    WM::do_frame();
  }

  cleanup(false);
  LOG::close();
  return 0;
}
