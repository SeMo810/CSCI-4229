#ifndef PEG_HPP_
#define PEG_HPP_

#include "../config.hpp"
#include "../math.hpp"

namespace GAME
{

struct Peg
{
  bool hit; // If the peg represents a hit
  math::Vec2i location; // The location on the game board
  math::Vec3f mlocation; // The model location in the world
  float velocity; // The current velocity of the peg, used for animations
  bool bounced; // For animations, if the peg is bouncing

  Peg() :
    hit(false), location(0, 0), mlocation(0, 0, 0), velocity(0), bounced(false)
  { }
};

extern bool initialize_pegs();
extern void update_pegs(float dtime);
extern void render_pegs();
extern void destroy_pegs();

extern void place_peg(const math::Vec2i& pos, bool hit);
extern bool is_peg_at(const math::Vec2i& pos);

}

#endif // PEG_HPP_
