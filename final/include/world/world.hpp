#ifndef WORLD_HPP_
#define WORLD_HPP_

#include "../config.hpp"
#include "../math.hpp"

namespace WORLD
{

extern bool create();
extern void destroy();

extern void update(float dtime);
extern void render();

}

#endif // WORLD_HPP_
