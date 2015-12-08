#include "graphics/lighting.hpp"
#include "graphics/ogl.hpp"

#define LIGHTCLAMP(x) ((x<0)?0:(x>1)?1:x)

namespace LIGHT
{

static float g_ambient = 0;
static float g_diffuse = 0;
static float g_specular = 0;
static math::Vec3f g_ambientcolor = math::Vec3f(1, 1, 1);
static math::Vec3f g_diffusecolor = math::Vec3f(1, 1, 1);
static math::Vec3f g_specularcolor = math::Vec3f(1, 1, 1);
static float g_shininess = 0;
static float g_timeofday = 0;

void initialize_lighting()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_SMOOTH);
}

void enable_lighting()
{
  glEnable(GL_LIGHTING);
}

void disable_lighting()
{
  glDisable(GL_LIGHTING);
}

void set_ambient_lighting(float inten)
{
  g_ambient = LIGHTCLAMP(inten);
}

void set_diffuse_lighting(float inten)
{
  g_diffuse = LIGHTCLAMP(inten);
}

void set_specular_lighting(float inten)
{
  g_specular = LIGHTCLAMP(inten);
}

float get_ambient_lighting()
{
  return g_ambient;
}

float get_diffuse_lighting()
{
  return g_diffuse;
}

float get_specular_lighting()
{
  return g_specular;
}

void set_ambient_color(const math::Vec3f& color)
{
  g_ambientcolor = color;
}

void set_diffuse_color(const math::Vec3f& color)
{
  g_diffusecolor = color;
}

void set_specular_color(const math::Vec3f& color)
{
  g_specularcolor = color;
}

math::Vec3f get_ambient_color()
{
  return g_ambientcolor;
}

math::Vec3f get_diffuse_color()
{
  return g_diffusecolor;
}

math::Vec3f get_specular_color()
{
  return g_specularcolor;
}

void set_shininess(float shin)
{
  g_shininess = shin;
}

float get_shininess()
{
  return g_shininess;
}

void applyMaterial(const Material& material)
{
  set_ambient_lighting(material.ambientIntensity);
  set_diffuse_lighting(material.diffuseIntensity);
  set_specular_lighting(material.specularIntensity);
  set_ambient_color(material.ambientColor);
  set_diffuse_color(material.diffuseColor);
  set_specular_color(material.specularColor);
  set_shininess(material.shininess);

  update_lighting();
}

void update_lighting()
{
  // TODO: Evetually allow for more than one light
  math::Vec3f a = g_ambientcolor * g_ambient;
  math::Vec3f d = g_diffusecolor * g_diffuse;
  math::Vec3f s = g_specularcolor * g_specular;
  float amb[] = { a.x, a.y, a.z, 1.0f };
  float dif[] = { d.x, d.y, d.z, 1.0f };
  float spc[] = { s.x, s.y, s.z, 1.0f };

  float angle = 10 + (g_timeofday * 160);
  float pos[] = { 50 * (float)COS(angle), 50 * (float)SIN(angle), 0, 1 };

  float shiny[] = { (float)pow(2.0, g_shininess) };
  float spec[] = { 1, 1, 1, 1 };

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spc);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
  glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
}

void set_time_of_day(float time)
{
  g_timeofday = LIGHTCLAMP(time);
}

void progress_time_of_day(float time)
{
  g_timeofday += time;
  g_timeofday = LIGHTCLAMP(g_timeofday);
}

float get_time_of_day()
{
  return g_timeofday;
}

}
