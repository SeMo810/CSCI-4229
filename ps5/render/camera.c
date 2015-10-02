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

/* Applies the given FPS_CAMERA to the MODELVIEW matrix. Use this every frame
    before drawing, after calling glLoadIdentity. */
void cm_apply_fps_camera(FPS_CAMERA camera)
{
  VEC3 fwd = cm_get_fps_camera_forward(camera);
  VEC3 target = vec3_add(camera.position, fwd);

  cm_apply_look_at(target, camera.position);
}

/* Get the forward vector for a fps camera. */
VEC3 cm_get_fps_camera_forward(FPS_CAMERA camera)
{
  double pitch = -camera.pitch + PI_O2;
  VEC3 fwd;
  fwd.x = -sin(camera.yaw) * sin(pitch);
  fwd.y = -cos(pitch);
  fwd.z = -cos(camera.yaw) * sin(pitch);
  return vec3_normalize(fwd);
}

/* Get the right vector for a fps camera. */
VEC3 cm_get_fps_camera_right(FPS_CAMERA camera)
{
  VEC3 fwd = cm_get_fps_camera_forward(camera);
  return vec3_normalize(vec3_cross(fwd, (VEC3){ 0, 1, 0 }));
}

/* Manually apply a look-at matrix. Will reset MODELVIEW automatically. */
void cm_apply_look_at(VEC3 target, VEC3 position)
{
  VEC3 forward = vec3_normalize(vec3_subtract(target, position));
  VEC3 right = vec3_cross((VEC3){ 0, 1, 0 }, forward);
  VEC3 up = vec3_normalize(vec3_cross(forward, right));

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

/* Manually apply a orthographic matrix. Will reset PROJECTION automatically. */
void cm_apply_ortho(float dim, float aspect)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-dim * aspect, dim * aspect, -dim, dim, -1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

/* Key update function for TARGET_CAMERA. */
void cm_target_camera_key(TARGET_CAMERA *camera, unsigned char k)
{
  if (!camera) return;

  switch (k)
  {
    case '=':
      camera->distance -= 0.15;
      break;
    case '-':
      camera->distance += 0.15;
      break;
    /* Reset the camera */
    case '0':
      camera->distance = 15.0;
      camera->yaw = PI_O4;
      camera->pitch = PI_O4 + 0.00001;
      break;
    default:
      return;
  }
}

/* Special update function for TARGET_CAMERA. */
void cm_target_camera_special(TARGET_CAMERA *camera, int key)
{
  if (!camera) return;

  switch (key)
  {
    case GLUT_KEY_UP:
      camera->pitch += 0.03;
      camera->pitch = CLAMPVAL(camera->pitch, -PI_O2, PI_O2);
      break;
    case GLUT_KEY_DOWN:
      camera->pitch -= 0.03;
      camera->pitch = CLAMPVAL(camera->pitch, -PI_O2, PI_O2);
      break;
    case GLUT_KEY_LEFT:
      camera->yaw -= 0.03;
      break;
    case GLUT_KEY_RIGHT:
      camera->yaw += 0.03;
      break;
    default:
      return;
  }
}

/* Key update function for FPS_CAMERA. */
void cm_fps_camera_key(FPS_CAMERA *camera, unsigned char k)
{
  if (!camera) return;

  VEC3 fwd = cm_get_fps_camera_forward(*camera);

  switch (k)
  {
    case 'w':
      camera->position = vec3_add(camera->position, vec3_multiply(fwd, 0.1));
      break;
    case 's':
      camera->position = vec3_add(camera->position, vec3_negate(vec3_multiply(fwd, 0.1)));
      break;
    case 'a':
      camera->yaw += 0.02;
      break;
    case 'd':
      camera->yaw -= 0.02;
      break;
    case 'q':
      camera->pitch += 0.02;
      camera->pitch = CLAMPVAL(camera->pitch, -PI_O2, PI_O2);
      break;
    case 'e':
      camera->pitch -= 0.02;
      camera->pitch = CLAMPVAL(camera->pitch, -PI_O2, PI_O2);
      break;
    /* Reset the camera */
    case '0':
      camera->position = (VEC3){ 10, 10, 10 };
      camera->yaw = PI_O4;
      camera->pitch = PI_O4;
      break;
    default:
      return;
  }
}

/* Special update function for FPS_CAMERA. */
void cm_fps_camera_special(FPS_CAMERA *camera, int key)
{
  if (!camera) return;

  switch (key)
  {
    default:
      return;
  }
}
