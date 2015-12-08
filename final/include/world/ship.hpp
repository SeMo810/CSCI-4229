#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "../config.hpp"
#include "../math.hpp"
#include "world/scripter.hpp"
#include "graphics/model_renderer.hpp"

// Namespace for gameplay related stuff
namespace GAME
{

#define SHIPTYPE_PATROL (0)
#define SHIPTYPE_SUBMARINE (1)
#define SHIPTYPE_DESTROYER (2)
#define SHIPTYPE_BATTLESHIP (3)
#define SHIPTYPE_CARRIER (4)

#define SHIPTEAM_ONE (1)
#define SHIPTEAM_TWO (2)

// Ship is in -Z direction (away from camera)
#define SHIPORIENT_NORTH (1)
// Ship is in +X direction (right of camera)
#define SHIPORIENT_EAST (2)
// Ship is in +Z direction (towards camera)
#define SHIPORIENT_SOUTH (3)
// Ship is in -X direction (left of camera)
#define SHIPORIENT_WEST (4)

template <int Type, int Health>
struct Ship
{
  int shipType;                   // Type of the ship (see SHIPTYPE_ defines)
  math::Vec2i position;           // Position of the front part of the ship
  int orientation;                // Orientation of the ship (see SHIPORIENT_ defines)
  math::Vec2i locations[Health];  // Array of locations that the ship occupies
  int team;                       // The team that the ship is on (see SHIPTEAM_ defines)
  int maxlife;                    // Maximum number of hits a ship can take before sinking
  int damage;                     // Total damage the ship currently has
  math::Vec3f mlocation;          // Location of the center of the ship, for rendering purposes
  float rotation;                 // Forward/back rotation of the ship for sinking
  float velocity;                 // Sinking velocity of the ship
  MODEL::Model *model;            // Model of the ship


  Ship() :
    shipType(Type), position(0, 0), orientation(SHIPORIENT_NORTH), locations{math::Vec2i(-1, -1)},
    team(SHIPTEAM_ONE), maxlife(Health), damage(0), mlocation(0, 0, 0), rotation(0), velocity(0),
    model(nullptr)
  {
    for (int i = 0; i < Health; ++i) locations[i] = math::Vec2i(-1);
  }
};

typedef Ship<SHIPTYPE_PATROL, 2> PatrolShip;
typedef Ship<SHIPTYPE_SUBMARINE, 3> SubmarineShip;
typedef Ship<SHIPTYPE_DESTROYER, 3> DestroyerShip;
typedef Ship<SHIPTYPE_BATTLESHIP, 4> BattleShip;
typedef Ship<SHIPTYPE_CARRIER, 5> CarrierShip;

template <int Type, int Health>
extern bool place_ship(Ship<Type, Health> *ship, const math::Vec2i& loc, int orient);
template <int Type, int Health>
extern bool ship_contains_position(Ship<Type, Health> *ship, const math::Vec2i& loc);

extern bool initialize_ships();
extern void update_ships(float dtime);
extern void render_ships();
extern void release_ships();

extern String last_ship_placement_error();

extern bool handle_ship_placement_opcode(const SCRIPT::ScriptInstruction& inst);
extern bool handle_fire_opcode(const SCRIPT::ScriptInstruction& inst);

}

#endif // SHIP_HPP_
