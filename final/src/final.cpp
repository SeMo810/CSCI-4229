#include <GLFW/glfw3.h>
#include "log.hpp"
#include "graphics/window_manager.hpp"

void normalexit()
{
  WM::terminate();

  LOG::warn("Logger is not closed normally during a normal exit.");
  LOG::close();
}

void terminateexit()
{
  WM::terminate();

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
  while (WM::is_window_open())
  {
    WM::do_frame();
  }

  WM::terminate();
  LOG::close();
  return 0;
}
