/*
 * Sphere drawing code taken from ex8 given on the class website.
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

#include "ogl.h"
#include "render_helper.h"

#define LEN 8192  /* Maximum length of text string. */

#define COS(x) cos(TO_RADIANS(x))
#define SIN(x) sin(TO_RADIANS(x))

/* Internal helper functions */
static void _apply_transforms(VEC3 pos, VEC3 scale, VEC3 rot)
{
  glTranslated(pos.x, pos.y, pos.z);
  glRotated(rot.x, 1.0, 0.0, 0.0);
  glRotated(rot.y, 0.0, 1.0, 0.0);
  glRotated(rot.z, 0.0, 0.0, 1.0);
  glScaled(scale.x, scale.y, scale.z);
}
static void _sphere_vertex(double theta, double phi)
{
  glColor3d(COS(theta)*COS(theta), SIN(phi)*SIN(phi), SIN(theta)*SIN(theta));
  double x = SIN(theta)*COS(phi)*0.5;
  double y = SIN(phi)*0.5;
  double z = COS(theta)*COS(phi)*0.5;
  glVertex3d(x, y, z);
  glNormal3d(x, y, z);
}
/* "side" is used to generate normals, 0 = bottom, 1 = side, 2 = top */
static void _polygon_vertex(double theta, double height, double step, int side)
{
  glColor3d(COS(theta)*COS(theta), SIN(theta)*SIN(theta), COS(theta)*SIN(theta));
  glVertex3d(0.5*COS(theta), height, 0.5*SIN(theta));
  switch (side)
  {
    case 0: /* Bottom face */
      glNormal3d(0.0, -1.0, 0.0);
      break;
    case 1: /* Side face */
      glNormal3d(COS(theta - step), 0.0, SIN(theta - step));
      break;
    case 2:
      glNormal3d(0.0, 1.0, 0.0);
      break;
    default:
      glNormal3d(0.0, 0.0, 0.0);
      break;
  }
}

/* Render world axes with the given sizes */
void rh_draw_world_axes(TARGET_CAMERA camera, VEC3 size)
{
  glPushMatrix();
  cm_apply_target_camera(camera);

  glBegin(GL_LINES);
    glColor3d(1.0, 1.0, 1.0);

    /* X-Axis */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(size.x, 0.0, 0.0);

    /* Y-Axis */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, size.y, 0.0);

    /* Z-Axis */
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, size.z);
  glEnd();

  VEC3 xs = { size.x, 0, 0 };
  VEC3 ys = { 0, size.y, 0 };
  VEC3 zs = { 0, 0, size.z };

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
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *ch++);
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
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *ch++);
}

/* Render a multi-colored cube at the given position, scale, and rotation. */
void rh_draw_cube(VEC3 pos, VEC3 scale, VEC3 rot)
{
  glPushMatrix();
  _apply_transforms(pos, scale, rot);

  glBegin(GL_QUADS);

    /* +X face */
    glColor3d(1, 0, 0); /* Red */
    glNormal3d(1.0, 0.0, 0.0);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(0.5, -0.5, -0.5);
    glVertex3d(0.5, -0.5, 0.5);
    /* -X face */
    glColor3d(0, 1, 0); /* Green */
    glNormal3d(-1.0, 0.0, 0.0);
    glVertex3d(-0.5, 0.5, -0.5);
    glVertex3d(-0.5, 0.5, 0.5);
    glVertex3d(-0.5, -0.5, 0.5);
    glVertex3d(-0.5, -0.5, -0.5);

    /* +Y face */
    glColor3d(0, 0, 1); /* Blue */
    glNormal3d(0.0, 1.0, 0.0);
    glVertex3d(-0.5, 0.5, -0.5);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(-0.5, 0.5, 0.5);
    /* -Y face */
    glColor3d(1, 1, 0); /* Yellow */
    glNormal3d(0.0, -1.0, 0.0);
    glVertex3d(0.5, -0.5, -0.5);
    glVertex3d(-0.5, -0.5, -0.5);
    glVertex3d(-0.5, -0.5, 0.5);
    glVertex3d(0.5, -0.5, 0.5);

    /* +Z face */
    glColor3d(1, 0, 1); /* Magenta */
    glNormal3d(0.0, 0.0, 1.0);
    glVertex3d(-0.5, 0.5, 0.5);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(0.5, -0.5, 0.5);
    glVertex3d(-0.5, -0.5, 0.5);
    /* -Z face */
    glColor3d(0, 1, 1); /* Cyan */
    glNormal3d(0.0, 0.0, -1.0);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(-0.5, 0.5, -0.5);
    glVertex3d(-0.5, -0.5, -0.5);
    glVertex3d(0.5, -0.5, -0.5);

  glEnd();

  glPopMatrix();
}

/* Render a multi-colored cube at the given position, scale, rotation, and quality.
    quality should be one of 1, 2, or 3. */
void rh_draw_sphere(VEC3 pos, VEC3 scale, VEC3 rot, int quality)
{
  glPushMatrix();
  _apply_transforms(pos, scale, rot);

  const int step = (quality == 1) ? 10 : ((quality == 2) ? 6 : 3);
  int theta, phi;

  /* South pole cap */
  glBegin(GL_TRIANGLE_FAN);
  _sphere_vertex(0, -90);
  for (theta = 0; theta <= 360; theta += step)
  {
    _sphere_vertex(theta, step - 90);
  }
  glEnd();

  /* Latitude bands */
  for (phi = step - 90; phi <= (90 - (2 * step)); phi += step)
  {
    glBegin(GL_QUAD_STRIP);
    for (theta = 0; theta <= 360; theta += step)
    {
       _sphere_vertex(theta, phi);
       _sphere_vertex(theta, phi + step);
    }
    glEnd();
  }

  /* North pole cap */
  glBegin(GL_TRIANGLE_FAN);
  _sphere_vertex(0, 90);
  for (theta = 0; theta <= 360; theta += step)
  {
    _sphere_vertex(theta, 90 - step);
  }
  glEnd();

  glPopMatrix();
}

/* Render a multi-colored 3d perfect polygon at the given position, scale, rotation,
    and number of sides. */
void rh_draw_extended_polygon(VEC3 pos, VEC3 scale, VEC3 rot, int sides)
{
  glPushMatrix();
  _apply_transforms(pos, scale, rot);

  sides = CLAMPVAL(sides, 2, 360);
  const double step = 360.0 / sides;
  double theta;

  /* Top Edge */
  glBegin(GL_TRIANGLE_FAN);
  glColor3d(1, 1, 1);
  glVertex3d(0, 0.5, 0);
  for (theta = 0.0; theta <= 360.05; theta += step)
  {
    _polygon_vertex(theta, 0.5, step, 2);
  }
  glEnd();

  /* Outer Ring */
  glBegin(GL_QUAD_STRIP);
  for (theta = 0.0; theta <= 360.05; theta += step)
  {
    _polygon_vertex(theta, 0.5, step, 1);
    _polygon_vertex(theta, -0.5, step, 1);
  }
  glEnd();

  /* Bottom Edge */
  glBegin(GL_TRIANGLE_FAN);
  glColor3d(1, 1, 1);
  glVertex3d(0, -0.5, 0);
  for (theta = 0.0; theta <= 360.05; theta += step)
  {
    _polygon_vertex(theta, -0.5, step, 0);
  }
  glEnd();

  glPopMatrix();
}
