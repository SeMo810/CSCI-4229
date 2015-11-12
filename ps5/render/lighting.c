#include "lighting.h"
#include "ogl.h"
#include <stdio.h>
#include <math.h>

#define COS(x) cos(TO_RADIANS(x))
#define SIN(x) sin(TO_RADIANS(x))

static void _sphere_vertex(double theta, double phi)
{
  glVertex3d(SIN(theta)*COS(phi)*0.15, SIN(phi)*0.15, COS(theta)*COS(phi)*0.15);
}

/* Prepare OpenGL lighting with the passed settings. */
void lht_prepare_lighting(LIGHTING_SETTINGS settings)
{
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glShadeModel(settings.smooth ? GL_SMOOTH : GL_FLAT);
}

/* Set the passed point light as the provided number light. */
void lht_set_light(int number, CYLINDER_LIGHT light)
{
  if (number < 0 || number >= GL_MAX_LIGHTS)
  {
    fprintf(stderr, "The number %d is not a valid OpenGL light. Max number of lights is %d.", number, GL_MAX_LIGHTS);
    return;
  }

  int lightnum = GL_LIGHT0 + number;

  VEC3 pos = lht_get_cylinder_light_position(light);
  float position[] = { pos.x, pos.y, pos.z, 1.0 };

  VEC3 a = vec3_multiply(light.ambientColor, light.ambientIntensity);
  VEC3 d = vec3_multiply(light.diffuseColor, light.diffuseIntensity);
  VEC3 s = vec3_multiply(light.specularColor, light.specularIntensity);
  float ambient[] = { a.x, a.y, a.z, 1.0 };
  float diffuse[] = { d.x, d.y, d.z, 1.0 };
  float specular[] = { s.x, s.y, s.z, 1.0 };

  glEnable(lightnum);
  glLightfv(lightnum, GL_AMBIENT, ambient);
  glLightfv(lightnum, GL_DIFFUSE, diffuse);
  glLightfv(lightnum, GL_SPECULAR, specular);
  glLightfv(lightnum, GL_POSITION, position);
}

/* Gets the light position of the cylinder light. */
VEC3 lht_get_cylinder_light_position(CYLINDER_LIGHT light)
{
  double posx = light.distance * cos(light.angle);
  double posz = light.distance * sin(light.angle);
  VEC3 pos = vec3_add(light.center, (VEC3){ posx, light.height, posz });
  return pos;
}

/* Draws a white sphere at the position of the passed light. */
void lht_draw_cylinder_light(CYLINDER_LIGHT light)
{
  VEC3 pos = lht_get_cylinder_light_position(light);

  glPushMatrix();
  glTranslated(pos.x, pos.y, pos.z);
  glColor3f(1, 1, 1);

  int th, ph;
  for (ph = -90; ph < 90; ph += 15)
  {
    glBegin(GL_QUAD_STRIP);
    for (th = 0; th <= 360; th += 30)
    {
       _sphere_vertex(th, ph);
       _sphere_vertex(th, ph + 15);
    }
    glEnd();
  }

  glPopMatrix();
}

/* Create the default lighting settings. */
LIGHTING_SETTINGS lht_create_default_lighting_settings()
{
  return (LIGHTING_SETTINGS){ .smooth = 1 };
}

/* Create a default cylindrical light. */
CYLINDER_LIGHT lht_create_default_cylinder_light()
{
  return (CYLINDER_LIGHT){
    .center = (VEC3){ 0, 0, 0 },
    .height = 0.0,
    .distance = 5.0,
    .angle = 0.0,
    .ambientColor = (VEC3){ 1, 1, 1 },
    .diffuseColor = (VEC3){ 1, 1, 1 },
    .specularColor = (VEC3){ 1, 1, 1 },
    .ambientIntensity = 0,
    .diffuseIntensity = 1,
    .specularIntensity = 0.25
  };
}
