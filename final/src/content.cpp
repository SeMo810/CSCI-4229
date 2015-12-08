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

MODEL::Model* load_model(const String& name)
{
  FILE *file = fopen(("content/" + name).c_str(), "r");
  if (!file)
  {
    LOG::error("Could not open the model file %s.", name.c_str());
    return nullptr;
  }

  std::vector<unsigned> iverts, inorms;
  std::vector<math::Vec3f> verts, norms;
  while (true)
  {
    char header[128];
    int res = fscanf(file, "%s", header);
    if (res == EOF)
      break;

    if (strcmp(header, "v") == 0)
    {
      math::Vec3f v;
      fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
      verts.push_back(v);
    }
    else if (strcmp(header, "vn") == 0)
    {
      math::Vec3f n;
      fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
      norms.push_back(n);
    }
    else if (strcmp(header, "f") == 0)
    {
      unsigned vi[3], ui[3], ni[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vi[0], &ui[0], &ni[0],
        &vi[1], &ui[1], &ni[1], &vi[2], &ui[2], &ni[2]);
      if (matches != 9)
      {
        LOG::error("Model file %s has incorrect face format.", name.c_str());
        return nullptr;
      }

      iverts.push_back(vi[0]);
      iverts.push_back(vi[1]);
      iverts.push_back(vi[2]);
      inorms.push_back(ni[0]);
      inorms.push_back(ni[1]);
      inorms.push_back(ni[2]);
    }
  }

  LOG::info("Loaded in the model %s with (%d) vertices and (%d) faces.", name.c_str(), verts.size(), iverts.size() / 3);

  GLuint res = MODEL::load_model_displaylist(verts, norms, iverts, inorms);
  if (res == 0)
    return nullptr;

  MODEL::Model *model = new MODEL::Model;
  model->listid = res;
  return model;
}

void free_model(MODEL::Model *model)
{
  MODEL::free_model_list(model);
  delete model;
}

}
