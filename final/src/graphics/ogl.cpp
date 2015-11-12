#include "graphics/ogl.hpp"
#include "config.hpp"
#include "log.hpp"

namespace OGL
{

GLenum LAST_GL_ERROR = GL_NO_ERROR;

bool check_error()
{
  bool haserror = false;
  while ((LAST_GL_ERROR = glGetError()) != GL_NO_ERROR)
  {
    haserror = true;
    const GLubyte *errstr = gluErrorString(LAST_GL_ERROR);
    LOG::error("OPENGL ERROR: (%d)\"%s\".", LAST_GL_ERROR, (const char*)errstr);
  }

  return haserror;
}

}
