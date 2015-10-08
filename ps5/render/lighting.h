#ifndef LIGHTING_H_
#define LIGHTING_H_

#include "vector.h"

/* Struct that represents a light that has a height, distance on xz plane, and
  angle, centered on a point. */
typedef struct
{
  VEC3 center;
  double height;
  double distance;
  double angle;
  VEC3 color;
  double ambientLevel;
} CYLINDER_LIGHT;

/* Struct that represents general lighting settings. */
typedef struct
{
  int smooth; /* 1 if the lighting should be smooth. */
} LIGHTING_SETTINGS;

/* Prepare OpenGL lighting with the passed settings. */
void lht_prepare_lighting(LIGHTING_SETTINGS settings);

/* Set the passed point light as the provided number light. */
void lht_set_light(int number, CYLINDER_LIGHT light);

/* Create the default lighting settings. */
LIGHTING_SETTINGS lht_create_default_lighting_settings();
/* Create a default cylindrical light. */
CYLINDER_LIGHT lht_create_default_cylinder_light();

#endif // LIGHTING_H_
