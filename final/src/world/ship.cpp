#include "world/ship.hpp"
#include "world/world.hpp"
#include "graphics/lighting.hpp"
#include "graphics/ogl.hpp"

static void apply_ship_transforms(const math::Vec2f& position, float rotation, float scale)
{
  glTranslated(position.x, 0.0, position.y);
  glRotated(rotation, 0.0, 1.0, 0.0);
  glScaled(scale, 0.8, 0.5);
}

namespace GAME
{

static PatrolShip g_patrolShip1 = PatrolShip();
static SubmarineShip g_submarineShip1 = SubmarineShip();
static DestroyerShip g_destroyerShip1 = DestroyerShip();
static BattleShip g_battleShip1 = BattleShip();
static CarrierShip g_carrierShip1 = CarrierShip();

template <int Type, int Health>
bool place_ship(Ship<Type, Health> *ship, const math::Vec2i& loc, int orient)
{
  // TODO: ERROR CHECKING AND POSITION VALIDATION
  int dir;
  if (orient == SHIPORIENT_WEST || orient == SHIPORIENT_EAST)
    dir = (orient == SHIPORIENT_WEST) ? -1 : 1;
  else
    dir = (orient == SHIPORIENT_NORTH) ? -1 : 1;

  if (orient == SHIPORIENT_WEST || orient == SHIPORIENT_EAST)
  {
    for (int i = 0; i < Health; ++i)
    {
      ship->locations[i] = math::Vec2i(loc.x + (i * dir), loc.y);
    }
  }
  else
  {
    for (int i = 0; i < Health; ++i)
    {
      ship->locations[i] = math::Vec2i(loc.x, loc.y + (i * dir));
    }
  }

  ship->position = loc;
  ship->orientation = orient;
  math::Vec2f min = ship->locations[0], max = ship->locations[Health - 1];
  math::Vec2f avg = (max + min) / 2.0f;
  ship->mlocation = avg + math::Vec2f(-WORLDTILEWIDTH / 2.0f, -WORLDTILEHEIGHT / 2.0f) + math::Vec2f(0.5f, 0.5f);

  return true;
}
template bool place_ship<SHIPTYPE_PATROL, 2>(Ship<SHIPTYPE_PATROL, 2> *ship, const math::Vec2i& loc, int orient);
template bool place_ship<SHIPTYPE_SUBMARINE, 3>(Ship<SHIPTYPE_SUBMARINE, 3> *ship, const math::Vec2i& loc, int orient);
template bool place_ship<SHIPTYPE_DESTROYER, 3>(Ship<SHIPTYPE_DESTROYER, 3> *ship, const math::Vec2i& loc, int orient);
template bool place_ship<SHIPTYPE_BATTLESHIP, 4>(Ship<SHIPTYPE_BATTLESHIP, 4> *ship, const math::Vec2i& loc, int orient);
template bool place_ship<SHIPTYPE_CARRIER, 5>(Ship<SHIPTYPE_CARRIER, 5> *ship, const math::Vec2i& loc, int orient);

template <int Type, int Health>
bool ship_contains_position(Ship<Type, Health> *ship, const math::Vec2i& loc)
{
  // TODO: ACTUALLY DO STUFF HERE
  return false;
}
template bool ship_contains_position<SHIPTYPE_PATROL, 2>(Ship<SHIPTYPE_PATROL, 2> *ship, const math::Vec2i& loc);
template bool ship_contains_position<SHIPTYPE_SUBMARINE, 3>(Ship<SHIPTYPE_SUBMARINE, 3> *ship, const math::Vec2i& loc);
template bool ship_contains_position<SHIPTYPE_DESTROYER, 3>(Ship<SHIPTYPE_DESTROYER, 3> *ship, const math::Vec2i& loc);
template bool ship_contains_position<SHIPTYPE_BATTLESHIP, 4>(Ship<SHIPTYPE_BATTLESHIP, 4> *ship, const math::Vec2i& loc);
template bool ship_contains_position<SHIPTYPE_CARRIER, 5>(Ship<SHIPTYPE_CARRIER, 5> *ship, const math::Vec2i& loc);

template <int Type, int Health>
void render_ship(Ship<Type, Health> *ship)
{
  float angle;
  switch (ship->orientation)
  {
    case SHIPORIENT_SOUTH:
      angle = 90;
      break;
    case SHIPORIENT_NORTH:
      angle = 270;
      break;
    case SHIPORIENT_WEST:
      angle = 180;
      break;
    default:
      angle = 0;
      break;
  }

  glPushMatrix();
  apply_ship_transforms(ship->mlocation, angle, ship->maxlife);

  // For now, just render a cube
  glBegin(GL_QUADS);

    /* +X face */
    glColor3d(1, 0, 0); /* Red */
    glNormal3d(1.0, 0.0, 0.0);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(0.5, -0.5, -0.5);
    glVertex3d(0.5, -0.5, 0.5);
    /* -X face */
    glColor3d(0, 1, 0); /* Green */
    glNormal3d(-1.0, 0.0, 0.0);
    glVertex3d(-0.5, 0.5, -0.5);
    glVertex3d(-0.5, 0.5, 0.5);
    glVertex3d(-0.5, -0.5, 0.5);
    glVertex3d(-0.5, -0.5, -0.5);

    /* +Y face */
    glColor3d(0, 0, 1); /* Blue */
    glNormal3d(0.0, 1.0, 0.0);
    glVertex3d(-0.5, 0.5, -0.5);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(-0.5, 0.5, 0.5);
    /* -Y face */
    glColor3d(1, 1, 0); /* Yellow */
    glNormal3d(0.0, -1.0, 0.0);
    glVertex3d(0.5, -0.5, -0.5);
    glVertex3d(-0.5, -0.5, -0.5);
    glVertex3d(-0.5, -0.5, 0.5);
    glVertex3d(0.5, -0.5, 0.5);

    /* +Z face */
    glColor3d(1, 0, 1); /* Magenta */
    glNormal3d(0.0, 0.0, 1.0);
    glVertex3d(-0.5, 0.5, 0.5);
    glVertex3d(0.5, 0.5, 0.5);
    glVertex3d(0.5, -0.5, 0.5);
    glVertex3d(-0.5, -0.5, 0.5);
    /* -Z face */
    glColor3d(0, 1, 1); /* Cyan */
    glNormal3d(0.0, 0.0, -1.0);
    glVertex3d(0.5, 0.5, -0.5);
    glVertex3d(-0.5, 0.5, -0.5);
    glVertex3d(-0.5, -0.5, -0.5);
    glVertex3d(0.5, -0.5, -0.5);

  glEnd();

  glPopMatrix();
}
template void render_ship<SHIPTYPE_PATROL, 2>(Ship<SHIPTYPE_PATROL, 2> *ship);
template void render_ship<SHIPTYPE_SUBMARINE, 3>(Ship<SHIPTYPE_SUBMARINE, 3> *ship);
template void render_ship<SHIPTYPE_DESTROYER, 3>(Ship<SHIPTYPE_DESTROYER, 3> *ship);
template void render_ship<SHIPTYPE_BATTLESHIP, 4>(Ship<SHIPTYPE_BATTLESHIP, 4> *ship);
template void render_ship<SHIPTYPE_CARRIER, 5>(Ship<SHIPTYPE_CARRIER, 5> *ship);

void initialize_ships()
{
  g_patrolShip1.team = SHIPTEAM_ONE;
  g_submarineShip1.team = SHIPTEAM_ONE;
  g_destroyerShip1.team = SHIPTEAM_ONE;
  g_battleShip1.team = SHIPTEAM_ONE;
  g_carrierShip1.team = SHIPTEAM_ONE;

  // Temporary, just explicitly place the ships
  place_ship(&g_patrolShip1, math::Vec2i(0, 0), SHIPORIENT_SOUTH);
  place_ship(&g_submarineShip1, math::Vec2i(6, 0), SHIPORIENT_SOUTH);
  place_ship(&g_destroyerShip1, math::Vec2i(0, 3), SHIPORIENT_EAST);
  place_ship(&g_battleShip1, math::Vec2i(4, 4), SHIPORIENT_EAST);
  place_ship(&g_carrierShip1, math::Vec2i(6, 7), SHIPORIENT_WEST);
}

void render_ships()
{
  LIGHT::enable_lighting();
  render_ship(&g_patrolShip1);
  render_ship(&g_submarineShip1);
  render_ship(&g_destroyerShip1);
  render_ship(&g_battleShip1);
  render_ship(&g_carrierShip1);
  LIGHT::disable_lighting();
}

}
