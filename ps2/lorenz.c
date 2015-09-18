#include <stdio.h>
#include <stdlib.h>
#include "lorenz.h"

/* The lorenz settings. */
static double lorenz_s = 10.0;
static double lorenz_b = 3.0;
static double lorenz_r = 28.0;

/* Current value for the steps. */
static LORENZ_COORDS *lorenz_current_coords = 0;

double lorenz_get_s_parameter(void)
{
  return lorenz_s;
}

double lorenz_get_b_parameter(void)
{
  return lorenz_b;
}

double lorenz_get_r_parameter(void)
{
  return lorenz_r;
}

void lorenz_get_parameters(double *s, double *b, double *r)
{
  if (s) { *s = lorenz_s; }
  if (b) { *b = lorenz_b; }
  if (r) { *r = lorenz_r; }
}

void lorenz_set_s_parameter(double s)
{
  lorenz_s = s;
}

void lorenz_set_b_parameter(double b)
{
  lorenz_b = b;
}

void lorenz_set_r_parameter(double r)
{
  lorenz_r = r;
}

void lorenz_set_parameters(double s, double b, double r)
{
  lorenz_s = s;
  lorenz_b = b;
  lorenz_r = r;
}

void lorenz_initialize(void)
{
  lorenz_shutdown();

  lorenz_current_coords = (LORENZ_COORDS*)malloc(sizeof(LORENZ_COORDS));
  if (!lorenz_current_coords)
  {
    fprintf(stderr, "Could not allocate space for the lorenz coordinates.");
    exit(-1);
  }

  lorenz_current_coords->x = 1.0;
  lorenz_current_coords->y = 1.0;
  lorenz_current_coords->z = 1.0;
}

void lorenz_shutdown(void)
{
  if (lorenz_current_coords)
  {
    free(lorenz_current_coords);
    lorenz_current_coords = 0;
  }
}

LORENZ_COORDS *lorenz_do_step(void)
{
  static double time_step = 0.001;
  double x = lorenz_current_coords->x;
  double y = lorenz_current_coords->y;
  double z = lorenz_current_coords->z;

  double dx = lorenz_s * (y - x);
  double dy = x * (lorenz_r - z) - y;
  double dz = (x * y) - (lorenz_b * z);
  x += (dx * time_step);
  y += (dy * time_step);
  z += (dz * time_step);

  lorenz_current_coords->x = x;
  lorenz_current_coords->y = y;
  lorenz_current_coords->z = z;
  return lorenz_current_coords;
}
