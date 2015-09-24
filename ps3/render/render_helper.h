#ifndef RENDER_HELPER_H_
#define RENDER_HELPER_H_

#include "camera.h"

/* Render world axes with the given camera and sizes. */
void rh_draw_world_axes(TARGET_CAMERA camera, VEC3 size);

/* Render text based on raster position */
void rh_draw_raster_text(VEC3 pos, const char *format, ...);
/* Render text based on window position */
void rh_draw_window_text(VEC2 pos, const char *format, ...);

/* Render a multi-colored cube at the given position, scale, and rotation. */
void rh_draw_cube(VEC3 pos, VEC3 scale, VEC3 rot);

#endif // RENDER_HELPER_H_
