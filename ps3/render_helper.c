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

/* Internal helper functions */
static void _apply_transforms(VEC3 pos, VEC3 scale, VEC3 rot)
{
  glScaled(scale.x, scale.y, scale.z);
  glRotated(rot.x, 1.0, 0.0, 0.0);
  glRotated(rot.y, 0.0, 1.0, 0.0);
  glRotated(rot.z, 0.0, 0.0, 1.0);
  glTranslated(pos.x, pos.y, pos.z);
}
static void _reset_transforms()
{
  glPushMatrix();
  glLoadIdentity();
}
static void _reload_transforms()
{
  glPopMatrix();
}

/* Render world axes with the given sizes */
void rh_draw_world_axes(double x, double y, double z)
{
  _reset_transforms();

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

  _reload_transforms();
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

/* Render a multi-colored cube at the given position, scale, and rotation. */
void rh_draw_cube(VEC3 pos, VEC3 scale, VEC3 rot)
{
  _reset_transforms();
  _apply_transforms(pos, scale, rot);

  glBegin(GL_QUADS);

    /* +X face */
    glColor3i(1, 0, 0); /* Red */
    glVertex3i(1, 1, 1);
    glVertex3i(1, 1, -1);
    glVertex3i(1, -1, -1);
    glVertex3i(1, -1, 1);
    /* -X face */
    glColor3i(0, 1, 0); /* Green */
    glVertex3i(-1, 1, -1);
    glVertex3i(-1, 1, 1);
    glVertex3i(-1, -1, 1);
    glVertex3i(-1, -1, -1);

    /* +Y face */
    glColor3i(0, 0, 1); /* Blue */
    glVertex3i(-1, 1, -1);
    glVertex3i(1, 1, -1);
    glVertex3i(1, 1, 1);
    glVertex3i(-1, 1, 1);
    /* -Y face */
    glColor3i(1, 1, 0); /* Yellow */
    glVertex3i(1, -1, -1);
    glVertex3i(-1, -1, -1);
    glVertex3i(-1, -1, 1);
    glVertex3i(1, -1, 1);

    /* +Z face */
    glColor3i(1, 0, 1); /* Magenta */
    glVertex3i(-1, 1, 1);
    glVertex3i(1, 1, 1);
    glVertex3i(1, -1, 1);
    glVertex3i(-1, -1, 1);
    /* -Z face */
    glColor3i(0, 1, 1); /* Cyan */
    glVertex3i(1, 1, -1);
    glVertex3i(-1, 1, -1);
    glVertex3i(-1, -1, -1);
    glVertex3i(1, -1, -1);

  glEnd();

  _reload_transforms();
}
