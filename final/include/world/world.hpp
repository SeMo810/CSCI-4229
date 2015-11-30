#ifndef WORLD_HPP_
#define WORLD_HPP_

#include "../config.hpp"
#include "../math.hpp"

#define WORLDTILEWIDTH 16
#define WORLDTILEHEIGHT 8
#define WORLDWIDTH ((WORLDTILEWIDTH * 4 + 1))
#define WORLDHEIGHT ((WORLDTILEHEIGHT * 4 + 1))

namespace WORLD
{

extern bool create();
extern void destroy();

extern void update(float dtime);
extern void render();

}

#endif // WORLD_HPP_
