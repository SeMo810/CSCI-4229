#include "texture.h"
#include <stdio.h>
#include <stdlib.h>

/*
 *  Reverse n bytes. TAKEN FROM EXAMPLE 14 FROM THE CLASS.
 */
static void Reverse(void* x,const int n)
{
   int k;
   char* ch = (char*)x;
   for (k=0;k<n/2;k++)
   {
      char tmp = ch[k];
      ch[k] = ch[n-1-k];
      ch[n-1-k] = tmp;
   }
}

/* Load a texture from a file. */
TEXTURE tex_load_from_file(const char* file)
{
  unsigned int   texture;    // Texture name
  FILE*          f;          // File pointer
  unsigned short magic;      // Image magic
  unsigned int   dx,dy,size; // Image dimensions
  unsigned short nbp,bpp;    // Planes and bits per pixel
  unsigned char* image;      // Image data
  unsigned int   k;          // Counter
  int            max;        // Maximum texture dimensions

  //  Open file
  f = fopen(file,"rb");
  if (!f)
  {
    fprintf(stderr, "COULD NOT OPEN THE FILE %s\n.", file);
    exit(1);
  }
  //  Check image magic
  if (fread(&magic,2,1,f)!=1)
  {
    fprintf(stderr, "Cannot read magic from %s\n",file);
    exit(1);
  }
  if (magic!=0x4D42 && magic!=0x424D)
  {
    fprintf(stderr, "Image magic not BMP in %s\n",file);
    exit(1);
  }
  //  Seek to and read header
  if (fseek(f,16,SEEK_CUR) || fread(&dx ,4,1,f)!=1 || fread(&dy ,4,1,f)!=1 ||
      fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
  {
    fprintf(stderr, "Cannot read header from %s\n",file);
    exit(1);
  }
  //  Reverse bytes on big endian hardware (detected by backwards magic)
  if (magic==0x424D)
  {
     Reverse(&dx,4);
     Reverse(&dy,4);
     Reverse(&nbp,2);
     Reverse(&bpp,2);
     Reverse(&k,4);
  }
  //  Check image parameters
  glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);
  if (dx<1 || dx>max)
  {
    fprintf(stderr, "%s image width %d out of range 1-%d\n",file,dx,max);
    exit(1);
  }
  if (dy<1 || dy>max)
  {
    fprintf(stderr, "%s image height %d out of range 1-%d\n",file,dy,max);
    exit(1);
  }
  if (nbp!=1)
  {
    fprintf(stderr, "%s bit planes is not 1: %d\n",file,nbp);
    exit(1);
  }
  if (bpp!=24)
  {
    fprintf(stderr, "%s bits per pixel is not 24: %d\n",file,bpp);
    exit(1);
  }
  if (k!=0)
  {
    fprintf(stderr, "%s compressed files not supported\n",file);
    exit(1);
  }
#ifndef GL_VERSION_2_0
  //  OpenGL 2.0 lifts the restriction that texture size must be a power of two
  for (k=1;k<dx;k*=2);
  if (k!=dx)
  {
    fprintf(stderr, "%s image width not a power of two: %d\n",file,dx);
    exit(1);
  }
  for (k=1;k<dy;k*=2);
  if (k!=dy)
  {
    fprintf(stderr, "%s image height not a power of two: %d\n",file,dy);
    exit(1);
  }
#endif

  //  Allocate image memory
  size = 3*dx*dy;
  image = (unsigned char*) malloc(size);
  if (!image)
  {
    fprintf(stderr, "Cannot allocate %d bytes of memory for image %s\n",size,file);
    exit(1);
  }
  //  Seek to and read image
  if (fseek(f,20,SEEK_CUR) || fread(image,size,1,f)!=1)
  {
    fprintf(stderr, "Error reading data from image %s\n",file);
    exit(1);
  }
  fclose(f);
  //  Reverse colors (BGR -> RGB)
  for (k=0;k<size;k+=3)
  {
     unsigned char temp = image[k];
     image[k]   = image[k+2];
     image[k+2] = temp;
  }

  //  Generate 2D texture
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);
  //  Copy image
  glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
  if (glGetError())
  {
    fprintf(stderr, "Error in glTexImage2D %s %dx%d\n",file,dx,dy);
    exit(1);
  }
  //  Scale linearly when image size doesn't match
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  //  Free image memory
  free(image);
  //  Return texture name
  TEXTURE tex = (TEXTURE)
  {
    .textureId = texture,
    .width = dx,
    .height = dy
  };
  return tex;
}
