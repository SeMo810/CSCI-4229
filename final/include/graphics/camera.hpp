#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "../config.hpp"
#include "../math.hpp"

namespace CAMERA
{

extern math::Vec2f get_camera_position();
extern void set_camera_position(math::Vec2f vec, bool force = false);
extern void move_camera(math::Vec2f vec, bool force = false);
extern void set_camera_distance(float dist, bool force = false);
extern void move_camera_distance(float dist, bool force = false);
extern void set_camera_rotation(float f);
extern void rotate_camera(float f);

extern void apply_camera_projection(bool perspective);
extern void apply_camera_transforms();

}

#endif // CAMERA_HPP_
