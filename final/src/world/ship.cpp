#include "world/ship.hpp"
#include "world/world.hpp"
#include "world/peg.hpp"
#include "graphics/lighting.hpp"
#include "graphics/ogl.hpp"
#include "log.hpp"
#include "content.hpp"

static void apply_ship_transforms(const math::Vec3f& position, float rotation, float tilt)
{
  glTranslated(position.x, position.y, position.z);
  glRotated(tilt, 1.0, 0.0, 0.0);
  glRotated(rotation, 0.0, 1.0, 0.0);
  glRotated(tilt * 0.25, 0.0, 0.0, 1.0);
}

static String formatted_string(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  char buffer[2048];
  vsprintf(buffer, format, args);
  va_end(args);

  return String(buffer);
}

namespace GAME
{

static PatrolShip g_patrolShip1 = PatrolShip();
static SubmarineShip g_submarineShip1 = SubmarineShip();
static DestroyerShip g_destroyerShip1 = DestroyerShip();
static BattleShip g_battleShip1 = BattleShip();
static CarrierShip g_carrierShip1 = CarrierShip();
static String g_lastError = String();

bool position_valid(const math::Vec2i& pos)
{
  bool bounds = pos.x >= 0 && pos.y >= 0 && pos.x < WORLDTILEWIDTH && pos.y < WORLDTILEHEIGHT;
  if (!bounds)
  {
    g_lastError = formatted_string("Error on position (%d, %d). Out of world bounds.", pos.x, pos.y);
    return false;
  }

  bool ship = !ship_contains_position(&g_patrolShip1, pos) &&
              !ship_contains_position(&g_submarineShip1, pos) &&
              !ship_contains_position(&g_destroyerShip1, pos) &&
              !ship_contains_position(&g_battleShip1, pos) &&
              !ship_contains_position(&g_carrierShip1, pos);
  if (!ship)
  {
    g_lastError = formatted_string("Error on position (%d, %d). Overlapping existing ship.", pos.x, pos.y);
    return false;
  }

  return true;
}

template <int Type, int Health>
bool place_ship(Ship<Type, Health> *ship, const math::Vec2i& loc, int orient)
{
  int dir;
  if (orient == SHIPORIENT_WEST || orient == SHIPORIENT_EAST)
    dir = (orient == SHIPORIENT_WEST) ? -1 : 1;
  else
    dir = (orient == SHIPORIENT_NORTH) ? -1 : 1;

  math::Vec2i templocs[Health];
  if (orient == SHIPORIENT_WEST || orient == SHIPORIENT_EAST)
  {
    for (int i = 0; i < Health; ++i)
    {
      templocs[i] = math::Vec2i(loc.x + (i * dir), loc.y);
    }
  }
  else
  {
    for (int i = 0; i < Health; ++i)
    {
      templocs[i] = math::Vec2i(loc.x, loc.y + (i * dir));
    }
  }

  for (int i = 0; i < Health; ++i)
  {
    if (!position_valid(templocs[i]))
    {
      LOG::error("Ship placement error. %s", g_lastError.c_str());
      return false;
    }
  }
  for (int i = 0; i < Health; ++i)
  {
    ship->locations[i] = templocs[i];
  }

  ship->position = loc;
  ship->orientation = orient;
  math::Vec2f min = ship->locations[0], max = ship->locations[Health - 1];
  math::Vec2f avg = (max + min) / 2.0f;
  ship->mlocation = math::Vec3f(avg.x, 0, avg.y) + math::Vec3f(-WORLDTILEWIDTH / 2.0f, 0, -WORLDTILEHEIGHT / 2.0f) + math::Vec3f(0.5f, 0, 0.5f);

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
  for (int i = 0; i < Health; ++i)
  {
    if (ship->locations[i] == loc)
      return true;
  }

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
      angle = 180;
      break;
    case SHIPORIENT_NORTH:
      angle = 0;
      break;
    case SHIPORIENT_WEST:
      angle = 270;
      break;
    default:
      angle = 90;
      break;
  }

  glPushMatrix();
  apply_ship_transforms(ship->mlocation, angle, ship->rotation);

  MODEL::draw_model(ship->model);

  glPopMatrix();
}
template void render_ship<SHIPTYPE_PATROL, 2>(Ship<SHIPTYPE_PATROL, 2> *ship);
template void render_ship<SHIPTYPE_SUBMARINE, 3>(Ship<SHIPTYPE_SUBMARINE, 3> *ship);
template void render_ship<SHIPTYPE_DESTROYER, 3>(Ship<SHIPTYPE_DESTROYER, 3> *ship);
template void render_ship<SHIPTYPE_BATTLESHIP, 4>(Ship<SHIPTYPE_BATTLESHIP, 4> *ship);
template void render_ship<SHIPTYPE_CARRIER, 5>(Ship<SHIPTYPE_CARRIER, 5> *ship);

template <int Type, int Health>
void update_ship(Ship<Type, Health> *ship, float dtime)
{
  if (ship->damage >= ship->maxlife)
  {
    if (ship->mlocation.y > -2)
    {
      if (ship->rotation < 35)
      {
        ship->rotation += (15 * dtime);
      }

      ship->velocity += (0.4 * dtime);
      ship->mlocation.y -= (ship->velocity * dtime);
    }
  }
}
template void update_ship<SHIPTYPE_PATROL, 2>(Ship<SHIPTYPE_PATROL, 2> *ship, float dtime);
template void update_ship<SHIPTYPE_SUBMARINE, 3>(Ship<SHIPTYPE_SUBMARINE, 3> *ship, float dtime);
template void update_ship<SHIPTYPE_DESTROYER, 3>(Ship<SHIPTYPE_DESTROYER, 3> *ship, float dtime);
template void update_ship<SHIPTYPE_BATTLESHIP, 4>(Ship<SHIPTYPE_BATTLESHIP, 4> *ship, float dtime);
template void update_ship<SHIPTYPE_CARRIER, 5>(Ship<SHIPTYPE_CARRIER, 5> *ship, float dtime);

bool initialize_ships()
{
  g_patrolShip1.team = SHIPTEAM_ONE;
  g_submarineShip1.team = SHIPTEAM_ONE;
  g_destroyerShip1.team = SHIPTEAM_ONE;
  g_battleShip1.team = SHIPTEAM_ONE;
  g_carrierShip1.team = SHIPTEAM_ONE;

  MODEL::Model *patrolModel = CONTENT::load_model("Patrol.obj");
  MODEL::Model *subModel = CONTENT::load_model("Submarine.obj");
  MODEL::Model *destroyerModel = CONTENT::load_model("Destroyer.obj");
  MODEL::Model *battleshipModel = CONTENT::load_model("Battleship.obj");
  MODEL::Model *carrierModel = CONTENT::load_model("Carrier.obj");
  if (!patrolModel || !subModel || !destroyerModel || !battleshipModel || !carrierModel)
  {
    LOG::error("Could not load ship model.");
    return false;
  }

  g_patrolShip1.model = patrolModel;
  g_submarineShip1.model = subModel;
  g_destroyerShip1.model = destroyerModel;
  g_battleShip1.model = battleshipModel;
  g_carrierShip1.model = carrierModel;
  return true;
}

void update_ships(float dtime)
{
  update_ship(&g_patrolShip1, dtime);
  update_ship(&g_submarineShip1, dtime);
  update_ship(&g_destroyerShip1, dtime);
  update_ship(&g_battleShip1, dtime);
  update_ship(&g_carrierShip1, dtime);
}

void render_ships()
{
  LIGHT::enable_lighting();
  float spec = LIGHT::get_specular_lighting();
  LIGHT::set_specular_lighting(0.1f);
  LIGHT::update_lighting();
  render_ship(&g_patrolShip1);
  render_ship(&g_submarineShip1);
  render_ship(&g_destroyerShip1);
  render_ship(&g_battleShip1);
  render_ship(&g_carrierShip1);
  LIGHT::set_specular_lighting(spec);
  LIGHT::update_lighting();
  LIGHT::disable_lighting();
}

void release_ships()
{
  CONTENT::free_model(g_patrolShip1.model);
  CONTENT::free_model(g_submarineShip1.model);
  CONTENT::free_model(g_destroyerShip1.model);
  CONTENT::free_model(g_battleShip1.model);
  CONTENT::free_model(g_carrierShip1.model);
}

String last_ship_placement_error()
{
  return g_lastError;
}

bool handle_ship_placement_opcode(const SCRIPT::ScriptInstruction& inst)
{
  math::Vec2i pos(inst.data.placex, inst.data.placey);
  int dir = inst.data.orientation;

  switch (inst.data.shiptype)
  {
    case SHIPTYPE_PATROL:
      return place_ship(&g_patrolShip1, pos, dir);
    case SHIPTYPE_SUBMARINE:
      return place_ship(&g_submarineShip1, pos, dir);
    case SHIPTYPE_DESTROYER:
      return place_ship(&g_destroyerShip1, pos, dir);
    case SHIPTYPE_BATTLESHIP:
      return place_ship(&g_battleShip1, pos, dir);
    case SHIPTYPE_CARRIER:
      return place_ship(&g_carrierShip1, pos, dir);
    default:
      return false;
  }
}

bool handle_fire_opcode(const SCRIPT::ScriptInstruction& inst)
{
  math::Vec2i pos(inst.data.firex, inst.data.firey);

  if (GAME::is_peg_at(pos))
    return false;

  bool hit = false;
  if (ship_contains_position(&g_patrolShip1, pos))
  {
    hit = true;
    g_patrolShip1.damage += 1;
  }
  else if (ship_contains_position(&g_submarineShip1, pos))
  {
    hit = true;
    g_submarineShip1.damage += 1;
  }
  else if (ship_contains_position(&g_destroyerShip1, pos))
  {
    hit = true;
    g_destroyerShip1.damage += 1;
  }
  else if (ship_contains_position(&g_battleShip1, pos))
  {
    hit = true;
    g_battleShip1.damage += 1;
  }
  else if (ship_contains_position(&g_carrierShip1, pos))
  {
    hit = true;
    g_carrierShip1.damage += 1;
  }

  GAME::place_peg(pos, hit);
  return true;
}

}
