#ifndef OGL_HPP_
#define OGL_HPP_

#define GL_GLEXT_PROTOTYPES
#ifdef PLATFORM_MAC
# include <OpenGL/glu.h>
#else
# ifdef PLATFORM_WINDOWS
#   ifndef NOMINMAX
#     define NOMINMAX
#   endif // NOMINMAX
#   include <windows.h>
# endif // PLATFORM_WINDOWS
# include <GL/glu.h>
#endif // PLATFORM_MAC

#include <GLFW/glfw3.h>

namespace OGL
{

extern GLenum LAST_GL_ERROR;

// Checks for an opengl error, and prints it and returns true if there is one
extern bool check_error();

}

#endif // OGL_HPP_
