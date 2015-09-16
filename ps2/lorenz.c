#include "lorenz.h"

static double lorenz_s = 10.0;
static double lorenz_b = 2.6666;
static double lorenz_r = 28.0;

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
