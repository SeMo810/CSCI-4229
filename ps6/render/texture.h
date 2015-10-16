#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "ogl.h"

typedef struct
{
  GLuint textureId;
  unsigned int width;
  unsigned int height;
} TEXTURE;

/* Load a texture from a file. */
TEXTURE tex_load_from_file(const char* file);

/* Set a texture as active. */
void tex_use_texture(TEXTURE tex);
/* Clear the active texture. */
void tex_clear_texture();

#endif // TEXTURE_H_
