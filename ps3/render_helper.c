#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include "render_helper.h"

#define LEN 8192  /* Maximum length of text string. */

/* Render world axes with the given sizes */
void rh_draw_world_axes(double x, double y, double z)
{
  glPushMatrix();
  glLoadIdentity();

  glBegin(GL_LINES);
    glColor3d(1.0, 1.0, 1.0);

    /* X-Axis */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(x, 0.0, 0.0);

    /* Y-Axis */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, y, 0.0);

    /* Z-Axis */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, z);
  glEnd();

  VEC3 xs = { x, 0, 0 };
  VEC3 ys = { 0, y, 0 };
  VEC3 zs = { 0, 0, z };

  rh_draw_raster_text(xs, "X");
  rh_draw_raster_text(ys, "Y");
  rh_draw_raster_text(zs, "Z");

  glPopMatrix();
}

/* Render text based on raster position */
void rh_draw_raster_text(VEC3 pos, const char *format, ...)
{
  glRasterPos3d(pos.x, pos.y, pos.z);

  char    buf[LEN];
  char*   ch = buf;
  va_list args;
  /* Turn the parameters into a character string */
  va_start(args, format);
  vsnprintf(buf, LEN, format, args);
  va_end(args);
  /* Display the characters one at a time at the current raster position */
  while (*ch)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
}

/* Render text based on window position */
void rh_draw_window_text(VEC2 pos, const char *format, ...)
{
  glWindowPos2i((int)pos.x, (int)pos.y);

  char    buf[LEN];
  char*   ch = buf;
  va_list args;
  /* Turn the parameters into a character string */
  va_start(args, format);
  vsnprintf(buf, LEN, format, args);
  va_end(args);
  /* Display the characters one at a time at the current raster position */
  while (*ch)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
}
