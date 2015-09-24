#ifndef VECTOR_H_
#define VECTOR_H_

#define PI 3.141592653589
#define PI_O2 (PI/2.0)
#define PI_O4 (PI/4.0)
#define TO_RADIANS(deg) (deg*PI/180.0)
#define TO_DEGREES(rad) (rad*180.0/PI)

#define CLAMPVAL(val,min,max) ((val)<(min)?(min):((val)>(max)?(max):(val)))

typedef struct
{
  double x, y;
} VEC2;

typedef struct
{
  double x, y, z;
} VEC3;

/* Unary math operations */
VEC2 vec2_negate(VEC2 v);
VEC3 vec3_negate(VEC3 v);

/* Binary math operations */
VEC2 vec2_add(VEC2 v1, VEC2 v2);
VEC3 vec3_add(VEC3 v1, VEC3 v2);
VEC2 vec2_subtract(VEC2 v1, VEC2 v2);
VEC3 vec3_subtract(VEC3 v1, VEC3 v2);
VEC2 vec2_multiply(VEC2 v, double s);
VEC3 vec3_multiply(VEC3 v, double s);
VEC2 vec2_divide(VEC2 v, double s);
VEC3 vec3_divide(VEC3 v, double s);

/* Vector operations */
double vec2_dot(VEC2 v1, VEC2 v2);
double vec3_dot(VEC3 v1, VEC3 v2);
VEC3 vec3_cross(VEC3 v1, VEC3 v2);
double vec2_length(VEC2 v);
double vec3_length(VEC3 v);
VEC2 vec2_normalize(VEC2 v);
VEC3 vec3_normalize(VEC3 v);

/* Vector casting */
VEC2 vec2_from_vec3(VEC3 v);
VEC3 vec3_from_vec2(VEC2 v);

#endif // VECTOR_H_
