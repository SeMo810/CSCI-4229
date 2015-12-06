#include "content.hpp"
#include "log.hpp"
#include "SOIL/SOIL.h"

GLuint load_missing_texture()
{
  static const unsigned char data[] =
  {
    255, 0, 255,
    0, 0, 0,
    0, 0, 0,
    255, 0, 255
  };

  GLuint handle;
  glGenTextures(1, &handle);
  if (handle == 0 || OGL::check_error())
  {
    LOG::error("Could not generate a new texture handle.");
    return 0;
  }

  glBindTexture(GL_TEXTURE_2D, handle);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glBindTexture(GL_TEXTURE_2D, 0);

  return handle;
}

namespace CONTENT
{

Texture* load_texture(const String& name, bool repeat, bool invert)
{
  GLuint hand = SOIL_load_OGL_texture(("content/" + name).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
    (repeat ? SOIL_FLAG_TEXTURE_REPEATS : 0) | (invert ? SOIL_FLAG_INVERT_Y : 0));

  if (hand == 0)
  {
    LOG::error("Could not load texture %s. Using missing texture. Error: %s.", name.c_str(), SOIL_last_result());
    hand = load_missing_texture();

    if (!hand)
    {
      LOG::error("Could not generate missing texture handle.");
      return nullptr;
    }

    Texture *tex = new Texture;
    tex->handle = hand;
    tex->width = 2;
    tex->height = 2;
    return tex;
  }

  Texture *tex = new Texture;
  tex->handle = hand;

  glBindTexture(GL_TEXTURE_2D, hand);
  int w, h;
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
  glBindTexture(GL_TEXTURE_2D, 0);
  tex->width = (GLuint)w;
  tex->height = (GLuint)h;

  return tex;
}

void free_texture(Texture*& tex)
{
  glDeleteTextures(1, &(tex->handle));
  delete tex;
  tex = nullptr;
}

void bind_texture(Texture* tex)
{
  if (tex)
    glBindTexture(GL_TEXTURE_2D, tex->handle);
  else
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
