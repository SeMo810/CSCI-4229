#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include "lorenz.h"

/* ================= GLOBAL VARIABLES ==================== */
static GLint window = 0;
static GLuint lorenzlist = 0;
/* ======================================================= */

#define LEN 8192  //  Maximum length of text string
static void
display_text(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args, format);
   vsnprintf(buf, LEN, format, args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

static void draw_axes(void)
{
  glBegin(GL_LINES);
  glColor3d(1.0, 1.0, 1.0);

  /* X-Axis */
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(5.0, 0.0, 0.0);

  /* Y-Axis */
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 5.0, 0.0);

  /* Z-Axis */
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 5.0);

  glEnd();

  glRasterPos3i(5, 0, 0);
  display_text("X");
  glRasterPos3i(0, 5, 0);
  display_text("Y");
  glRasterPos3i(0, 0, 5);
  display_text("Z");
}

static void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glLoadIdentity();
  glTranslatef(0, 0, -25);

  draw_axes();

  glCallList(lorenzlist);

  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

static void idle(void)
{
  glutPostRedisplay();
}

static void key(unsigned char k, int x, int y)
{
  glutPostRedisplay();
}

static void special(int k, int x, int y)
{
  glutPostRedisplay();
}

static void reshape(int w, int h)
{
  GLfloat ratio = (GLfloat)w / (GLfloat)h;

  glViewport(0, 0, (GLint)w, (GLint)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  /*glFrustum(-1.0, 1.0, -ratio, ratio, 0.1, 60.0);*/
  gluPerspective(45.0, ratio, 0.1, 1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void create_lorenz_attractor(void)
{
  glNewList(lorenzlist, GL_COMPILE);

  /* Create the lorenz attractor */
  glBegin(GL_LINE_STRIP);
  int i;
  for (i = 0; i < 50000; ++i)
  {
    LORENZ_COORDS *coords = lorenz_do_step();
    glVertex3d(coords->x / 10, coords->y / 10, coords->z / 10);
  }
  glEnd();

  glEndList();
}

static void init(int argc, char** argv)
{
  lorenz_initialize();

  lorenzlist = glGenLists(1);
  create_lorenz_attractor();

  glEnable(GL_DEPTH_TEST);
}

static void shutdown(void)
{
  lorenz_shutdown();

  glDeleteLists(lorenzlist, 1);
  glutDestroyWindow(window);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(300, 300);
  window = glutCreateWindow("Lorenz Attractor");
  init(argc, argv);

  glutDisplayFunc(draw);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);

  glutMainLoop();
  shutdown();
  return 0;
}
