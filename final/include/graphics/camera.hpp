#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "../config.hpp"
#include "../math.hpp"

namespace CAMERA
{

extern math::Vec2f get_camera_position();
extern void set_camera_position(math::Vec2f vec, bool force = false);
extern void move_camera(math::Vec2f vec, bool force = false);

}

#endif // CAMERA_HPP_
