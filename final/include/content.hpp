#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include "config.hpp"
#include "graphics/ogl.hpp"
#include "graphics/model_renderer.hpp"

namespace CONTENT
{

struct Texture
{
  GLuint handle;
  GLuint width;
  GLuint height;

  Texture():
    handle(0), width(0), height(0)
  { }
};

extern Texture* load_texture(const String& name, bool repeat = true, bool invert = false);
extern void free_texture(Texture*& tex);
extern void bind_texture(Texture* tex);

extern MODEL::Model* load_model(const String& name);
extern void free_model(MODEL::Model *model);

}

#endif // CONTENT_HPP_
