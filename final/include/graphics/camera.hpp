#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "../config.hpp"
#include "../math.hpp"

namespace CAMERA
{

extern math::Vec2f get_camera_position();
extern void set_camera_position(math::Vec2f vec, bool force = false);
extern void move_camera(math::Vec2f vec, bool force = false);
extern void set_camera_distnace(float dist, bool force = false);
extern void move_camera_distance(float dist, bool force = false);

extern void apply_camera_projection(bool perspective);
extern void apply_camera_transforms();

}

#endif // CAMERA_HPP_
