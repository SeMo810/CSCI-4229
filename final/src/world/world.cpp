#include "world/world.hpp"
#include "log.hpp"
#include "graphics/ogl.hpp"

#define WORLDTILEWIDTH 16
#define WORLDTILEHEIGHT 8
#define WORLDWIDTH ((WORLDTILEWIDTH * 4 + 1))
#define WORLDHEIGHT ((WORLDTILEHEIGHT * 4 + 1))

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
  static const int worldwidth = WORLDWIDTH;
  static const int worldheight = WORLDHEIGHT;
  static const int worldsize = worldwidth * worldheight;
  g_waterData = (float*)malloc(sizeof(float) * worldsize);
  if (!g_waterData)
  {
    LOG::error("Could not allocate space for the water height data.");
    return false;
  }

  // Populate the water heights
  for (int i = 0; i < worldwidth; ++i)
  {
    float height = 0.05f * (float)sin(i / 4.0);
    for (int j = 0; j < worldheight; ++j)
    {
      int index = (j * worldwidth) + i;
      g_waterData[index] = height;
    }
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

  static const int worldwidth = WORLDWIDTH;
  static const int worldheight = WORLDHEIGHT;
  static float lastTime = 0.0f;
  lastTime += (dtime * 5);

  // Update the water heights
  for (int i = 0; i < worldwidth; ++i)
  {
    float height = 0.05f * (float)sin((i + lastTime) / 4.0);
    for (int j = 0; j < worldheight; ++j)
    {
      int index = (j * worldwidth) + i;
      g_waterData[index] = height;
    }
  }
}

// TODO: MAKE THIS NOT SO HORRIFICALLY INEFFICIENT. IT HURTS.
void render()
{
  if (!g_worldCreated)
    return;

  glBegin(GL_QUADS);
  static const float widthfactor = (float)WORLDTILEWIDTH / (float)WORLDWIDTH;
  static const float heightfactor = (float)WORLDTILEHEIGHT / (float)WORLDHEIGHT;
  static const int worldwidth = WORLDWIDTH - 1;
  static const int worldheight = WORLDHEIGHT - 1;
  static const int htilew = WORLDTILEWIDTH / 2;
  static const int htileh = WORLDTILEHEIGHT / 2;

  for (int i = 0; i < worldwidth; ++i)
  {
    for (int j = 0; j < worldheight; ++j)
    {
      int tl = (j * worldwidth) + i;
      int tr = (j * worldwidth) + i + 1;
      int br = ((j + 1) * worldwidth) + i + 1;
      int bl = ((j + 1) * worldwidth) + i;

      float tlh = g_waterData[tl];
      float trh = g_waterData[tr];
      float brh = g_waterData[br];
      float blh = g_waterData[bl];

      float x1 = (i * widthfactor) - htilew;
      float x2 = ((i + 1) * widthfactor) - htilew;
      float y1 = (j * heightfactor) - htileh;
      float y2 = ((j + 1) * heightfactor) - htileh;

      float c1 = 1.0f - (tlh + 0.05);
      float c2 = 1.0f - (trh + 0.05);
      float c3 = 1.0f - (brh + 0.05);
      float c4 = 1.0f - (blh + 0.05);

      glColor3f(0, 0, c1);
      glVertex3f(x1, tlh, y1);
      glColor3f(0, 0, c2);
      glVertex3f(x2, trh, y1);
      glColor3f(0, 0, c3);
      glVertex3f(x2, brh, y2);
      glColor3f(0, 0, c4);
      glVertex3f(x1, blh, y2);
    }
  }

  glEnd();
}

}
