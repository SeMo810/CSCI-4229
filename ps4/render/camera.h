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

/* Defines a camera that can be controlled like a first person camera. */
typedef struct
{
  VEC3 position;
  double yaw;
  double pitch;
} FPS_CAMERA;

/* Applies the given TARGET_CAMERA to the MODELVIEW matrix. Use this every frame
    before drawing, after calling glLoadIdentity. */
void cm_apply_target_camera(TARGET_CAMERA camera);
/* Applies the given FPS_CAMERA to the MODELVIEW matrix. Use this every frame
    before drawing, after calling glLoadIdentity. */
void cm_apply_fps_camera(FPS_CAMERA camera);

/* Get the forward vector for a fps camera. */
VEC3 cm_get_fps_camera_forward(FPS_CAMERA camera);
/* Get the right vector for a fps camera. */
VEC3 cm_get_fps_camera_right(FPS_CAMERA camera);

/* Manually apply a look-at matrix. Will reset MODELVIEW automatically. */
void cm_apply_look_at(VEC3 target, VEC3 position);
/* Manually apply a projection matrix. Will reset PROJECTION automatically. */
void cm_apply_perspective(float fov, float aspect);
/* Manually apply a orthographic matrix. Will reset PROJECTION automatically. */
void cm_apply_ortho(float dim, float aspect);

/* Key update function for TARGET_CAMERA. */
void cm_target_camera_key(TARGET_CAMERA *camera, unsigned char k);
/* Special update function for TARGET_CAMERA. */
void cm_target_camera_special(TARGET_CAMERA *camera, int key);

/* Key update function for FPS_CAMERA. */
void cm_fps_camera_key(FPS_CAMERA *camera, unsigned char k);
/* Special update function for FPS_CAMERA. */
void cm_fps_camera_special(FPS_CAMERA *camera, int key);

#endif // CAMERA_H_
