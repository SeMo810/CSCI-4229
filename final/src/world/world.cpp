#include "world/world.hpp"
#include "log.hpp"
#include "graphics/ogl.hpp"

#define WORLDWIDTH ((16 * 4 + 1))
#define WORLDHEIGHT ((8 * 4 + 1))

namespace WORLD
{

static bool g_worldCreated = false;
static float *g_waterData = nullptr;
static GLint g_waterDisplayList = 0;

bool create()
{
  if (g_worldCreated)
  {
    LOG::warn("World has already been created, and cannot be created twice.");
    return true;
  }

  // Try to allocate the world data
  int worldwidth = WORLDWIDTH;
  int worldheight = WORLDHEIGHT;
  int worldsize = worldwidth * worldheight;
  g_waterData = (float*)malloc(sizeof(float) * worldsize);
  if (!g_waterData)
  {
    LOG::error("Could not allocate space for the water height data.");
    return false;
  }

  // Try to reserve gl lists
  if (!(g_waterDisplayList = glGenLists(1)) || OGL::check_error())
  {
    LOG::error("Could not generate the display list for the water.");
    return false;
  }

  // Populate the water heights
  for (int i = 0; i < worldwidth; ++i)
  {
    float height = 0.25f * (float)sin(i / 4.0);
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

  glDeleteLists(g_waterDisplayList, 1);

  free(g_waterData);
  g_waterData = nullptr;

  g_worldCreated = false;
}

void update(float dtime)
{
  int worldwidth = WORLDWIDTH;
  int worldheight = WORLDHEIGHT;

  // Update the water heights
  for (int i = 0; i < worldwidth; ++i)
  {
    float height = 0.25f * (float)sin((i + dtime) / 4.0);
    for (int j = 0; j < worldheight; ++j)
    {
      int index = (j * worldwidth) + i;
      g_waterData[index] = height;
    }
  }
}

void render()
{

}

}
