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
/* Render a multi-colored cube at the given position, scale, rotation, and quality.
    quality should be one of 1, 2, or 3. */
void rh_draw_sphere(VEC3 pos, VEC3 scale, VEC3 rot, int quality);
/* Render a multi-colored 3d perfect polygon at the given position, scale, rotation,
    and number of sides. Sides must be an integer from 2 to 360. */
void rh_draw_extended_polygon(VEC3 pos, VEC3 scale, VEC3 rot, int sides);
/* Render a cone. Sides must be an integer from 2 to 360. */
void rh_draw_cone(VEC3 pos, VEC3 scale, VEC3 rot, int sides);

#endif // RENDER_HELPER_H_
