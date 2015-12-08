#include "log.hpp"
#include "graphics/camera.hpp"
#include "graphics/ogl.hpp"

#define CAMERA_LIMITS_X (8.f)
#define CAMERA_LIMITS_Z (4.f)
#define CAMERA_LIMITS_DISTMIN (5.0f)
#define CAMERA_LIMITS_DISTMAX (25.0f)

namespace CAMERA
{

static math::Vec2f g_cameraPosition = math::Vec2f();
static float g_cameraDistance = 15.0f;
static float g_cameraRotation = 0;

math::Vec2f get_camera_position()
{
  return g_cameraPosition;
}

math::Vec3f get_camera_offset()
{
  using namespace math;
  Vec3f target(g_cameraPosition.x, 0.0f, g_cameraPosition.y);
  Vec3f forward(-sin(g_cameraRotation), -.5, -cos(g_cameraRotation));
  forward = glm::normalize(forward);
  Vec3f diff = (-forward) * g_cameraDistance;
  Vec3f pos = diff + target;
  return pos;
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

void set_camera_distance(float dist, bool force)
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

void set_camera_rotation(float f)
{
  g_cameraRotation = f;
}

void rotate_camera(float f)
{
  g_cameraRotation += f;
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

  Vec3f target(g_cameraPosition.x, 0.0f, g_cameraPosition.y);
  Vec3f forward(-sin(g_cameraRotation), -.5, -cos(g_cameraRotation));
  forward = glm::normalize(forward);
  Vec3f diff = (-forward) * g_cameraDistance;
  Vec3f pos = diff + target;
  Vec3f right = glm::cross(forward, Vec3f(0, 1, 0));
  Vec3f up = -glm::cross(forward, right);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z);
}

}
