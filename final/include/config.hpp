#ifndef CONFIG_H_
#define CONFIG_H_

#include <cstdlib>
#include <cstring>
#include <exception>
#include <cstdarg>
#include <cstdio>
#include <math.h>
#include <fstream>
#include <time.h>

typedef std::basic_string<char> String;

#define PI 3.141592653589
#define PI_O2 (PI/2.0)
#define PI_O4 (PI/4.0)
#define TO_RADIANS(deg) (deg*PI/180.0)
#define TO_DEGREES(rad) (rad*180.0/PI)

#define CLAMPVAL(val,min,max) ((val)<(min)?(min):((val)>(max)?(max):(val)))

#define COS(x) cos(TO_RADIANS(x))
#define SIN(x) sin(TO_RADIANS(x))

// Namespace for error stuff
namespace ERR
{

extern int GLFW_ERROR_CODE;
extern String GLFW_ERROR_STRING;

}

#endif // CONFIG_H_
