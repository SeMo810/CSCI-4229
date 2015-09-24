#ifndef RENDER_HELPER_H_
#define RENDER_HELPER_H_

/* Vector 2 type */
typedef struct
{
  double x;
  double y;
} VEC2;

/* Vector/Color 3 type */
typedef struct
{
  double x;
  double y;
  double z;
} VEC3, COLOR3;

/* Vector/Color 4 type */
typedef struct
{
  double x;
  double y;
  double z;
  double w;
} VEC4, COLOR4;

/* Create a look-at view matrix */
void rh_camera_look_at(VEC3 target, VEC3 position);
/* Create a projection matrix */
void rh_camera_projection(double fov, double aspect);
/* Create an orthographic projection matrix */
void rh_camera_orthographic();

/* Render world axes with the given sizes */
void rh_draw_world_axes(double x, double y, double z);

/* Render text based on raster position */
void rh_draw_raster_text(VEC3 pos, const char *format, ...);
/* Render text based on window position */
void rh_draw_window_text(VEC2 pos, const char *format, ...);

/* Render a multi-colored cube at the given position, scale, and rotation. */
void rh_draw_cube(VEC3 pos, VEC3 scale, VEC3 rot);

#endif // RENDER_HELPER_H_
