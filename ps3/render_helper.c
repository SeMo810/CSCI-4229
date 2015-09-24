#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

#include "ogl.h"
#include "render_helper.h"

#define LEN 8192  /* Maximum length of text string. */

static const VEC3 UP_VECTOR = { 0, 1, 0 };

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
static VEC3 _cross_vec3(VEC3 v1, VEC3 v2)
{
  VEC3 result;
  result.x = v1.y * v2.z - v1.z * v2.y;
  result.y = v1.x * v2.z - v1.z * v2.x;
  result.z = v1.x * v2.y - v1.y * v2.x;
  return result;
}
static VEC3 _subtract_vec3(VEC3 v1, VEC3 v2)
{
  VEC3 result;
  result.x = v1.x - v2.x;
  result.y = v1.y - v2.y;
  result.z = v1.z - v2.z;
  return result;
}
static double _length_vec3(VEC3 v)
{
  double len = v.x * v.x + v.y * v.y + v.z * v.z;
  return sqrt(len);
}
static VEC3 _normalize_vec3(VEC3 v)
{
  double len = _length_vec3(v);
  v.x /= len;
  v.y /= len;
  v.z /= len;
  return v;
}

/* Create a look-at view matrix */
void rh_camera_look_at(VEC3 target, VEC3 position)
{
  VEC3 forward = _normalize_vec3(_subtract_vec3(target, position));
  VEC3 right = _cross_vec3(forward, UP_VECTOR);
  VEC3 up = _normalize_vec3(_cross_vec3(right, forward));

  glMatrixMode(GL_PROJECTION);
  gluLookAt(position.x, position.y, position.z, target.x, target.y, target.z, up.x, up.y, up.z);
  glMatrixMode(GL_MODELVIEW);
}

/* Create a projection matrix */
void rh_camera_projection(double fov, double aspect)
{
  glMatrixMode(GL_PROJECTION);
  gluPerspective(fov, aspect, 0.1, 10000.0);
  glMatrixMode(GL_MODELVIEW);
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
