#include "graphics/gui.hpp"
#include "graphics/ogl.hpp"

#define LEN 1024

namespace GUI
{

void draw_text(const math::Vec2f& pos, const char *format, ...)
{
  glWindowPos2i((int)pos.x, 891 - (int)pos.y);

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

void draw_large_text(const math::Vec2f& pos, const char *format, ...)
{
  glWindowPos2i((int)pos.x, 891 - (int)pos.y);

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

void draw_raster_text(const math::Vec3f& pos, const char *format, ...)
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

void draw_large_raster_text(const math::Vec3f& pos, const char *format, ...)
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

void set_text_color(float r, float g, float b)
{
  glColor3f(r, g, b);
}

void reset_text_color()
{
  glColor3f(1, 1, 1);
}

void draw_controls()
{
  math::Vec2f pos(5, 5);
  draw_text(pos, "Controls:");
  pos.y += 15;
  draw_text(pos, "    W/A/S/D moves camera");
  pos.y += 15;
  draw_text(pos, "    Mouse Wheel zooms camera");
  pos.y += 15;
  draw_text(pos, "    , and . changes the ambient lighting settings");
  pos.y += 15;
  draw_text(pos, "    ; and ' changes the diffuse lighting settings");
  pos.y += 15;
  draw_text(pos, "    [ and ] changes the time of day");
}

}
