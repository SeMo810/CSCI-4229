#include "graphics/camera.hpp"

#define CAMERA_LIMITS_X (8.f)
#define CAMERA_LIMITS_Z (4.f)

namespace CAMERA
{

static math::Vec2f g_cameraPosition = math::Vec2f();

math::Vec2f get_camera_position()
{
  return g_cameraPosition;
}

void set_camera_position(math::Vec2f vec, bool force)
{
  g_cameraPosition = vec;
  if (!force)
  {
    if (g_cameraPosition.x > CAMERA_LIMITS_X)
      g_cameraPosition.x = CAMERA_LIMITS_X;
    if (g_cameraPosition.x < -CAMERA_LIMITS_X)
      g_cameraPosition.x = -CAMERA_LIMITS_X;
    if (g_cameraPosition.y > CAMERA_LIMITS_Z)
      g_cameraPosition.y = CAMERA_LIMITS_Z;
    if (g_cameraPosition.y < -CAMERA_LIMITS_Z)
      g_cameraPosition.y = -CAMERA_LIMITS_Z;
  }
}

void move_camera(math::Vec2f vec, bool force)
{
  g_cameraPosition += vec;
  if (!force)
  {
    if (g_cameraPosition.x > CAMERA_LIMITS_X)
      g_cameraPosition.x = CAMERA_LIMITS_X;
    if (g_cameraPosition.x < -CAMERA_LIMITS_X)
      g_cameraPosition.x = -CAMERA_LIMITS_X;
    if (g_cameraPosition.y > CAMERA_LIMITS_Z)
      g_cameraPosition.y = CAMERA_LIMITS_Z;
    if (g_cameraPosition.y < -CAMERA_LIMITS_Z)
      g_cameraPosition.y = -CAMERA_LIMITS_Z;
  }
}

}
