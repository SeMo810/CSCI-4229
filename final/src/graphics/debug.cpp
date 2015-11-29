#include "graphics/debug.hpp"
#include "graphics/ogl.hpp"

namespace DEBUG
{

void render_world_axes()
{
  glBegin(GL_LINES);

  /* X-Axis */
  glColor3d(1.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(1.0, 0.0, 0.0);
  /* Y-Axis */
  glColor3d(0.0, 1.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 1.0, 0.0);
  /* Z-Axis */
  glColor3d(0.0, 0.0, 1.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 1.0);

  glEnd();
}

}
