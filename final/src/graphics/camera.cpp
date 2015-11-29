#include "graphics/camera.hpp"
#include "graphics/ogl.hpp"

#define CAMERA_LIMITS_X (8.f)
#define CAMERA_LIMITS_Z (4.f)
#define CAMERA_LIMITS_DISTMIN (1.0f)
#define CAMERA_LIMITS_DISTMAX (10.0f)

namespace CAMERA
{

static math::Vec2f g_cameraPosition = math::Vec2f();
static float g_cameraDistance = 5.0f;

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

void set_camera_distnace(float dist, bool force)
{
  g_cameraDistance = dist;
  if (!force)
  {
    if (g_cameraDistance < CAMERA_LIMITS_DISTMIN)
      g_cameraDistance = CAMERA_LIMITS_DISTMIN;
    if (g_cameraDistance > CAMERA_LIMITS_DISTMAX)
      g_cameraDistance = CAMERA_LIMITS_DISTMAX;
  }
}

void move_camera_distance(float dist, bool force)
{
  g_cameraDistance += dist;
  if (!force)
  {
    if (g_cameraDistance < CAMERA_LIMITS_DISTMIN)
      g_cameraDistance = CAMERA_LIMITS_DISTMIN;
    if (g_cameraDistance > CAMERA_LIMITS_DISTMAX)
      g_cameraDistance = CAMERA_LIMITS_DISTMAX;
  }
}

void apply_camera_projection(bool perspective)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (perspective)
    gluPerspective(45.0, 16.0 / 9.0, 0.01, 10000.0);
  else
  {
    float dim = 5.0;
    float aspect = 16.0 / 9.0;
    glOrtho(-dim * aspect, dim * aspect, -dim, dim, -1.0, 10000.0);
  }

  glMatrixMode(GL_MODELVIEW);
}

void apply_camera_transforms()
{
  using namespace math;

  static const float sqrt2 = sqrt(2.0f);
  Vec3f fwd = Vec3f(0.0f, -sqrt2, sqrt2);
  Vec3f tgt = Vec3f(g_cameraPosition.x, 0.0f, g_cameraPosition.y);
  Vec3f diff = (-fwd) * g_cameraDistance;
  Vec3f pos = tgt + diff;

  Vec3f up = glm::cross(fwd, Vec3f(1.0f, 0.0f, 0.0f));

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(pos.x, pos.y, pos.z, tgt.x, tgt.y, tgt.z, up.x, up.y, up.z);
}

}
