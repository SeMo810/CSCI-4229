#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector.h"

/* Defines a camera that can rotate freely around a target point. */
typedef struct
{
  VEC3 target;
  double yaw; /* Rotation around the y-axis. */
  double pitch; /* Rotation above or below the xz-plane. */
  double distance; /* Distance from the target point. */
} TARGET_CAMERA;

/* Applies the given TARGET_CAMERA to the MODELVIEW matrix. Use this every frame
    before drawing, after calling glLoadIdentity. */
void cm_apply_target_camera(TARGET_CAMERA camera);

/* Manually apply a look-at matrix. Will reset MODELVIEW automatically. */
void cm_apply_look_at(VEC3 target, VEC3 position);
/* Manually apply a projection matrix. Will reset PROJECTION automatically. */
void cm_apply_perspective(float fov, float aspect);

#endif // CAMERA_H_
