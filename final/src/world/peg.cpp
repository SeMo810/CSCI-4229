#include "world/peg.hpp"
#include "world/world.hpp"
#include "graphics/ogl.hpp"
#include "graphics/lighting.hpp"
#include "log.hpp"

static void apply_peg_transforms(const math::Vec3f& position)
{
  glTranslated(position.x, position.y, position.z);
}

namespace GAME
{

static Peg* g_pegs[WORLDTILESIZE / 2] = { nullptr };
static int g_pegCount = 0;
static GLuint g_pegDisplayList = 0;

bool initialize_pegs()
{
  g_pegDisplayList = glGenLists(1);
  if (g_pegDisplayList == 0 || OGL::check_error())
  {
    LOG::error("Could not generate a display list id for the pegs.");
    return false;
  }

  glNewList(g_pegDisplayList, GL_COMPILE);
  glBegin(GL_QUADS);
  /* +X face */
  glVertex3d(0.2, 0.25, 0.2);
  glVertex3d(0.2, 0.25, -0.2);
  glVertex3d(0.2, -0.25, -0.2);
  glVertex3d(0.2, -0.25, 0.2);
  /* -X face */
  glVertex3d(-0.2, 0.25, -0.2);
  glVertex3d(-0.2, 0.25, 0.2);
  glVertex3d(-0.2, -0.25, 0.2);
  glVertex3d(-0.2, -0.25, -0.2);
  /* +Y face */
  glVertex3d(-0.2, 0.25, -0.2);
  glVertex3d(0.2, 0.25, -0.2);
  glVertex3d(0.2, 0.25, 0.2);
  glVertex3d(-0.2, 0.25, 0.2);
  /* -Y face */
  glVertex3d(0.2, -0.25, -0.2);
  glVertex3d(-0.2, -0.25, -0.2);
  glVertex3d(-0.2, -0.25, 0.2);
  glVertex3d(0.2, -0.25, 0.2);
  /* +Z face */
  glVertex3d(-0.2, 0.25, 0.2);
  glVertex3d(0.2, 0.25, 0.2);
  glVertex3d(0.2, -0.25, 0.2);
  glVertex3d(-0.2, -0.25, 0.2);
  /* -Z face */
  glVertex3d(0.2, 0.25, -0.2);
  glVertex3d(-0.2, 0.25, -0.2);
  glVertex3d(-0.2, -0.25, -0.2);
  glVertex3d(0.2, -0.25, -0.2);

  /* +X face */
  glVertex3d(0.1, 0.25, 0.1);
  glVertex3d(0.1, 0.25, -0.1);
  glVertex3d(0.1, -1.25, -0.1);
  glVertex3d(0.1, -1.25, 0.1);
  /* -X face */
  glVertex3d(-0.1, 0.25, -0.1);
  glVertex3d(-0.1, 0.25, 0.1);
  glVertex3d(-0.1, -1.25, 0.1);
  glVertex3d(-0.1, -1.25, -0.1);
  /* +Y face */
  glVertex3d(-0.1, 0.25, -0.1);
  glVertex3d(0.1, 0.25, -0.1);
  glVertex3d(0.1, 0.25, 0.1);
  glVertex3d(-0.1, 0.25, 0.1);
  /* -Y face */
  glVertex3d(0.1, -1.25, -0.1);
  glVertex3d(-0.1, -1.25, -0.1);
  glVertex3d(-0.1, -1.25, 0.1);
  glVertex3d(0.1, -1.25, 0.1);
  /* +Z face */
  glVertex3d(-0.1, 0.25, 0.1);
  glVertex3d(0.1, 0.25, 0.1);
  glVertex3d(0.1, -1.25, 0.1);
  glVertex3d(-0.1, -1.25, 0.1);
  /* -Z face */
  glVertex3d(0.1, 0.25, -0.1);
  glVertex3d(-0.1, 0.25, -0.1);
  glVertex3d(-0.1, -1.25, -0.1);
  glVertex3d(0.1, -1.25, -0.1);
  glEnd();
  glEndList();

  if (OGL::check_error())
  {
    LOG::error("Could not compile the peg display list.");
    return false;
  }

  return true;
}

void update_pegs(float dtime)
{
  for (int i = 0; i < g_pegCount; ++i)
  {
    Peg *peg = g_pegs[i];

    if (peg->mlocation.y > 1)
    {
      peg->velocity += (5 * dtime);
      peg->mlocation.y -= (peg->velocity * dtime);

      if (peg->mlocation.y <= 1)
      {
        if (peg->bounced)
        {
          peg->mlocation.y = 1;
          peg->velocity = 0;
        }
        else
        {
          peg->bounced = true;
          peg->velocity = -2;
          peg->mlocation.y = 1.001;
        }
      }
    }
  }
}

void render_pegs()
{
  LIGHT::disable_lighting();

  for (int i = 0; i < g_pegCount; ++i)
  {
    Peg *peg = g_pegs[i];
    if (peg->hit)
      glColor3f(1, 0, 0);
    else
      glColor3f(1, 1, 1);

    glPushMatrix();
    apply_peg_transforms(peg->mlocation);

    glCallList(g_pegDisplayList);
    glPopMatrix();
  }
}

void destroy_pegs()
{
  glDeleteLists(g_pegDisplayList, 1);
  g_pegDisplayList = 0;

  for (int i = 0; i < g_pegCount; ++i)
  {
    delete g_pegs[i];
    g_pegs[i] = nullptr;
  }
  g_pegCount = 0;
}

void place_peg(const math::Vec2i& pos, bool hit)
{
  if (is_peg_at(pos))
    return;

  g_pegs[g_pegCount++] = new Peg;
  Peg *peg = g_pegs[g_pegCount - 1];

  peg->hit = hit;
  peg->location = pos;

  float x = pos.x - (WORLDTILEWIDTH / 2)+ 0.5;
  float z = pos.y - (WORLDTILEHEIGHT / 2) + 0.5;
  peg->mlocation = math::Vec3f(x, 5, z);
}

bool is_peg_at(const math::Vec2i& pos)
{
  for (int i = 0; i < g_pegCount; ++i)
  {
    if (g_pegs[i]->location == pos)
      return true;
  }

  return false;
}

}
