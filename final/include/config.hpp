#ifndef CONFIG_H_
#define CONFIG_H_

#include <cstdlib>
#include <string>
#include <exception>
#include <cstdarg>
#include <cstdio>
#include <math.h>
#include <fstream>
#include <time.h>

typedef std::basic_string<char> String;

// Namespace for error stuff
namespace ERR
{

extern int GLFW_ERROR_CODE;
extern String GLFW_ERROR_STRING;

}

#endif // CONFIG_H_
