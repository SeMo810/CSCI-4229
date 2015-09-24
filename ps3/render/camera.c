#include <math.h>
#include <stdlib.h>
#include "camera.h"
#include "ogl.h"

#define NOT_DUMB_ABS(val) ((val)<0?-(val):(val))

/* Applies the given TARGET_CAMERA to the MODELVIEW matrix. Use this every frame
    before drawing, after calling glLoadIdentity. */
void cm_apply_target_camera(TARGET_CAMERA camera)
{
  double pitch = -camera.pitch + PI_O2;
  VEC3 pos;
  pos.x = camera.distance * sin(camera.yaw) * sin(pitch);
  pos.y = camera.distance * cos(pitch);
  pos.z = camera.distance * cos(camera.yaw) * sin(pitch);

  cm_apply_look_at(camera.target, pos);
}

/* Manually apply a look-at matrix. Will reset MODELVIEW automatically. */
void cm_apply_look_at(VEC3 target, VEC3 position)
{
  VEC3 forward = vec3_normalize(vec3_subtract(target, position));
  VEC3 right = vec3_cross(forward, (VEC3){ 0, 1, 0 });
  VEC3 up = vec3_normalize(vec3_cross(forward, right));
  double ang = atan(NOT_DUMB_ABS(forward.y) / sqrt(forward.x * forward.x + forward.z * forward.z));
  if (ang >= PI_O4)
  {
    up = vec3_negate(up);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(position.x, position.y, position.z, target.x, target.y, target.z, up.x, up.y, up.z);
}

/* Manually apply a projection matrix. Will reset PROJECTION automatically. */
void cm_apply_perspective(float fov, float aspect)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, aspect, 0.1, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}
