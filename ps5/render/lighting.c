#include "lighting.h"
#include "ogl.h"

/* Prepare OpenGL lighting with the passed settings. */
void lht_prepare_lighting(LIGHTING_SETTINGS settings)
{

}

/* Set the passed point light as the provided number light. */
void lht_set_light(int number, CYLINDER_LIGHT light)
{

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
    .distance = 10.0,
    .angle = 0.0,
    .color = (VEC3){ 1, 1, 1 },
    .ambientLevel = 0
  };
}
