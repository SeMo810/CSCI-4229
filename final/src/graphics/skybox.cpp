#include "graphics/skybox.hpp"
#include "graphics/lighting.hpp"
#include "graphics/ogl.hpp"
#include "graphics/camera.hpp"
#include "content.hpp"
#include "log.hpp"

namespace CAMERA
{

static void _apply_camera_offset()
{
  math::Vec3f pos = CAMERA::get_camera_offset();
  glTranslated(pos.x, pos.y, pos.z);
}

static CONTENT::Texture *g_skyTexture = nullptr;
static GLuint g_listID = 0;

bool load_skybox()
{
  g_skyTexture = CONTENT::load_texture("skybox.jpg");
  if (!g_skyTexture)
  {
    LOG::error("Could not load texture for the skybox.");
    return false;
  }

  g_listID = glGenLists(1);
  if (!g_listID)
  {
    LOG::error("Could not generate display list for skybox.");
    return false;
  }

  static const double third = 1.0 / 3.0;
  static const double third2 = 2.0 / 3.0 - 0.01;

  glNewList(g_listID, GL_COMPILE);
  glBegin(GL_QUADS);

    /* +X face */
    glTexCoord2d(0.5, third);
    glVertex3d(0.5, 0.5, 0.5);
    glTexCoord2d(0.25, third);
    glVertex3d(0.5, 0.5, -0.5);
    glTexCoord2d(0.25, third2);
    glVertex3d(0.5, -0.5, -0.5);
    glTexCoord2d(0.5, third2);
    glVertex3d(0.5, -0.5, 0.5);
    /* -X face */
    glTexCoord2d(1, third);
    glVertex3d(-0.5, 0.5, -0.5);
    glTexCoord2d(0.75, third);
    glVertex3d(-0.5, 0.5, 0.5);
    glTexCoord2d(0.75, third2);
    glVertex3d(-0.5, -0.5, 0.5);
    glTexCoord2d(1, third2);
    glVertex3d(-0.5, -0.5, -0.5);

    /* -Y face */
    glTexCoord2d(third, third2);
    glVertex3d(0.55, -0.5, -0.55);
    glVertex3d(-0.55, -0.5, -0.55);
    glVertex3d(-0.55, -0.5, 0.55);
    glVertex3d(0.55, -0.5, 0.55);

    /* +Z face */
    glTexCoord2d(0.75, third);
    glVertex3d(-0.5, 0.5, 0.5);
    glTexCoord2d(0.5, third);
    glVertex3d(0.5, 0.5, 0.5);
    glTexCoord2d(0.5, third2);
    glVertex3d(0.5, -0.5, 0.5);
    glTexCoord2d(0.75, third2);
    glVertex3d(-0.5, -0.5, 0.5);
    /* -Z face */
    glTexCoord2d(0.25, third);
    glVertex3d(0.5, 0.5, -0.5);
    glTexCoord2d(0, third);
    glVertex3d(-0.5, 0.5, -0.5);
    glTexCoord2d(0, third2);
    glVertex3d(-0.5, -0.5, -0.5);
    glTexCoord2d(0.25, third2);
    glVertex3d(0.5, -0.5, -0.5);

  glEnd();
  glEndList();

  return true;
}

#define FABS(x) ((x)<0?-(x):(x))

void draw_skybox()
{
  LIGHT::disable_lighting();
  glDisable(GL_DEPTH_TEST);
  glPushMatrix();
  _apply_camera_offset();
  CONTENT::bind_texture(g_skyTexture);

  float tod = LIGHT::get_time_of_day();
  float diff = FABS(0.5 - tod);
  if (diff < 0.4)
    glColor3d(1, 1, 1);
  else
  {
    diff = (0.1 - FABS(diff - 0.5)) * 10;
    glColor3d(1, 1 - (diff * .2), 1 - (diff * .2));
  }

  glCallList(g_listID);
  glColor3d(1, 1, 1);
  CONTENT::bind_texture(nullptr);
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
  LIGHT::enable_lighting();
}

void free_skybox()
{
  CONTENT::free_texture(g_skyTexture);
  glDeleteLists(1, g_listID);
}

}
