#include "world/world.hpp"
#include "log.hpp"
#include "graphics/lighting.hpp"
#include "graphics/ogl.hpp"

namespace WORLD
{

static bool g_worldCreated = false;
static float *g_waterData = nullptr;

bool create()
{
  if (g_worldCreated)
  {
    LOG::warn("World has already been created, and cannot be created twice.");
    return true;
  }

  // Try to allocate the world data
  static const int worldwidth = WORLDWIDTH * 2;
  g_waterData = (float*)malloc(sizeof(float) * worldwidth);
  if (!g_waterData)
  {
    LOG::error("Could not allocate space for the water height data.");
    return false;
  }

  // Populate the water heights
  for (int i = 0; i < worldwidth; ++i)
  {
    float height = 0.05f * (float)sin(i / 4.0);
    g_waterData[i] = height;
  }

  g_worldCreated = true;
  return true;
}

void destroy()
{
  if (!g_worldCreated)
  {
    LOG::warn("Cannot destroy the world if it is not created.");
    return;
  }

  free(g_waterData);
  g_waterData = nullptr;

  g_worldCreated = false;
}

void update(float dtime)
{
  if (!g_worldCreated)
    return;

  static const int worldwidth = WORLDWIDTH * 2;
  static float lastTime = 0.0f;
  lastTime += (dtime * 2);

  // Update the water heights
  for (int i = 0; i < worldwidth; ++i)
  {
    float height = 0.05f * (float)sin((i + lastTime) / 4.0);
    g_waterData[i] = height;
  }
}

// TODO: MAKE THIS NOT SO HORRIFICALLY INEFFICIENT. IT HURTS.
void render()
{
  if (!g_worldCreated)
    return;

  static const float widthfactor = 2.0f * (float)WORLDTILEWIDTH / (float)WORLDWIDTH;
  static const int worldwidth = WORLDWIDTH - 1;
  static const int htilew = WORLDTILEWIDTH / 2;
  static const int htileh = WORLDTILEHEIGHT / 2;

  // Draw the ocean floor
  LIGHT::enable_lighting();
  glBegin(GL_QUADS);
  glColor3f(0.6f, 0.463f, 0.196f);
  glVertex3f(-WORLDTILEWIDTH, -2, -WORLDTILEHEIGHT);
  glVertex3f(WORLDTILEWIDTH, -2, -WORLDTILEHEIGHT);
  glVertex3f(WORLDTILEWIDTH, -2, WORLDTILEHEIGHT);
  glVertex3f(-WORLDTILEWIDTH, -2, WORLDTILEHEIGHT);
  glEnd();

  // Draw the ocean surface
  glDepthMask(0);
  glBegin(GL_QUADS);
  for (int i = 0; i <= worldwidth; ++i)
  {
    float h1 = g_waterData[i];
    float h2 = g_waterData[i + 1];

    float x1 = (i * widthfactor) - WORLDTILEWIDTH;
    float x2 = ((i + 1) * widthfactor) - WORLDTILEWIDTH;

    float c1 = 1.0f - (h1 + 0.05) * 3;
    float c2 = 1.0f - (h2 + 0.05) * 3;

    glColor4f(0, 0, c1, 0.8f);
    glVertex3f(x1, h1, WORLDTILEHEIGHT);
    glColor4f(0, 0, c2, 0.8f);
    glVertex3f(x2, h2, WORLDTILEHEIGHT);
    glColor4f(0, 0, c2, 0.8f);
    glVertex3f(x2, h2, -WORLDTILEHEIGHT);
    glColor4f(0, 0, c1, 0.8f);
    glVertex3f(x1, h1, -WORLDTILEHEIGHT);
  }
  glEnd();
  LIGHT::disable_lighting();

  // Draw the x-horizontal grid
  glBegin(GL_LINES);
  glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
  for (int x = -htilew; x <= htilew; ++x)
  {
    glVertex3f(x, -0.1, -htileh);
    glVertex3f(x, -0.1, htileh);
  }
  // Draw the z-horizontal grid
  for (int z = -htileh; z <= htileh; ++z)
  {
    glVertex3f(-htilew, -0.1, z);
    glVertex3f(htilew, -0.1, z);
  }
  glEnd();

  // Draw the board separator
  glBegin(GL_LINES);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glVertex3f(0, -0.1, htileh);
  glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
  glVertex3f(0, 3, htileh);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glVertex3f(0, -0.1, -htileh);
  glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
  glVertex3f(0, 3, -htileh);
  glEnd();
  glBegin(GL_QUADS);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glVertex3f(0, -0.1, htileh);
  glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
  glVertex3f(0, 3, htileh);
  glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
  glVertex3f(0, 3, -htileh);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glVertex3f(0, -0.1, -htileh);
  glEnd();
  glDepthMask(1);
}

}
