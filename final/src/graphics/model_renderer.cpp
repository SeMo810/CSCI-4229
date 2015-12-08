#include "graphics/model_renderer.hpp"
#include "log.hpp"

namespace MODEL
{

GLuint load_model_displaylist(std::vector<math::Vec3f>& verts, std::vector<math::Vec3f>& norms, std::vector<unsigned int>& iverts, std::vector<unsigned int>& inorms)
{
  GLuint listid = glGenLists(1);
  if (listid == 0)
  {
    LOG::error("Could not reserve a display list id for a model.");
    return 0;
  }

  glNewList(listid, GL_COMPILE);
  glBegin(GL_TRIANGLES);

  glColor3d(0.5, 0.5, 0.5);
  for (unsigned i = 0; i < iverts.size(); ++i)
  {
    unsigned vi = iverts[i];
    unsigned ni = inorms[i];
    math::Vec3f vert = verts[vi - 1];
    math::Vec3f norm = norms[ni - 1];

    glNormal3d(norm.x, norm.y, norm.z);
    glVertex3d(vert.x, vert.y, vert.z);
  }

  glEnd();
  glEndList();

  return listid;
}

void draw_model(Model *model)
{
  glCallList(model->listid);
}

void free_model_list(Model *model)
{
  glDeleteLists(1, model->listid);
}

}
