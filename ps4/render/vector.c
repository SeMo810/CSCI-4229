#include <math.h>
#include "vector.h"

/* Unary math operations */
VEC2 vec2_negate(VEC2 v)
{
  return (VEC2){ -v.x, -v.y };
}

VEC3 vec3_negate(VEC3 v)
{
  return (VEC3){ -v.x, -v.y, -v.z };
}

/* Binary math operations */
VEC2 vec2_add(VEC2 v1, VEC2 v2)
{
  return (VEC2){ v1.x + v2.x, v1.y + v2.y };
}

VEC3 vec3_add(VEC3 v1, VEC3 v2)
{
  return (VEC3){ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

VEC2 vec2_subtract(VEC2 v1, VEC2 v2)
{
  return (VEC2){ v1.x - v2.x, v1.y - v2.y };
}

VEC3 vec3_subtract(VEC3 v1, VEC3 v2)
{
  return (VEC3){ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

VEC2 vec2_multiply(VEC2 v, double s)
{
  return (VEC2){ v.x * s, v.y * s };
}

VEC3 vec3_multiply(VEC3 v, double s)
{
  return (VEC3){ v.x * s, v.y * s, v.z * s };
}

VEC2 vec2_divide(VEC2 v, double s)
{
  return (VEC2){ v.x / s, v.y / s };
}

VEC3 vec3_divide(VEC3 v, double s)
{
  return (VEC3){ v.x / s, v.y / s, v.z / s };
}

/* Vector operations */
double vec2_dot(VEC2 v1, VEC2 v2)
{
  return v1.x * v2.x + v1.y * v2.y;
}

double vec3_dot(VEC3 v1, VEC3 v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

VEC3 vec3_cross(VEC3 v1, VEC3 v2)
{
  return (VEC3){ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

double vec2_length(VEC2 v)
{
  return sqrt(v.x * v.x + v.y * v.y);
}

double vec3_length(VEC3 v)
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

VEC2 vec2_normalize(VEC2 v)
{
  return vec2_divide(v, vec2_length(v));
}

VEC3 vec3_normalize(VEC3 v)
{
  return vec3_divide(v, vec3_length(v));
}

/* Vector casting */
VEC2 vec2_from_vec3(VEC3 v)
{
  return (VEC2){ v.x, v.y };
}

VEC3 vec3_from_vec2(VEC2 v)
{
  return (VEC3){ v.x, v.y, 0 };
}
