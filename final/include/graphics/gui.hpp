#ifndef GUI_HPP_
#define GUI_HPP_

#include "../config.hpp"
#include "../math.hpp"

namespace GUI
{

extern void draw_text(const math::Vec2f& pos, const char *format, ...);
extern void draw_large_text(const math::Vec2f& pos, const char *format, ...);

extern void draw_raster_text(const math::Vec3f& pos, const char *format, ...);
extern void draw_large_raster_text(const math::Vec3f& pos, const char *format, ...);

extern void set_text_color(float r, float g, float b);
extern void reset_text_color();

extern void draw_controls();

}

#endif // GUI_HPP_
