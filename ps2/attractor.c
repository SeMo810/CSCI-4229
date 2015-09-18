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

#define CLAMP_VALUE(x,y,z) (x<y?y:(x>z?z:x))

/* ================= GLOBAL VARIABLES ==================== */
static GLint window = 0;
static GLuint lorenzlist = 0;
static int yaw = 0;
static int pitch = 0;
static float distance = 25.0f;
static int colorscheme = 1;
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

static void draw_gui(void)
{
  glColor3d(1.0, 1.0, 1.0);
  glWindowPos2i(2, 2);
  double s, b, r;
  lorenz_get_parameters(&s, &b, &r);
  display_text("R = %.2f", r);
  glWindowPos2i(2, 17);
  display_text("B = %.2f", b);
  glWindowPos2i(2, 32);
  display_text("S = %.2f", s);
  glWindowPos2i(2, 47);
  display_text("Dist = %.2f", distance);
  glWindowPos2i(2, 62);
  switch (colorscheme)
  {
    case 1:
      display_text("Color = POSITION DEPENDENT 1");
      break;
    case 2:
      display_text("Color = POSITION DEPENDENT 2");
      break;
    case 3:
      display_text("Color = LINEAR");
      break;
    default:
      display_text("Color = BROKEN");
      break;
  }
}

static void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glLoadIdentity();
  glTranslatef(0, 0, -distance);
  glRotatef(pitch, 1, 0, 0);
  glRotatef(yaw, 0, 1, 0);

  draw_axes();
  glCallList(lorenzlist);
  draw_gui();

  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

static void idle(void)
{
  glutPostRedisplay();
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
    double x = coords->x / 10, y = coords->y / 10, z = coords->z / 10;
    double r = 1, g = 1, b = 1;
    if (colorscheme == 1)
    {
      /* I have no idea what I did here, but it ended up looking cool. */
      r = (sin(x * y) + 1) * 0.5;
      g = (sin(x * z) + cos(x * z)) * 0.5;
      b = (cos(y * z) + 1) * 0.5;
    }
    else if (colorscheme == 2)
    {
      /* Simply colored on absolute distance from (0, 0, 0) */
      double d = x*x + y*y + z*z;
      r = (sin(d) + 1) * 0.5;
      g = (sin(d) + cos(d)) * 0.5;
      b = (cos(d) + 1) * 0.5;
    }
    else
    {
      /* Linear along the steps */
      double d = i * 3.141592 / 180.0;
      r = (sin(d) + 1) * 0.5;
      g = (sin(d) + cos(d)) * 0.5;
      b = (cos(d) + 1) * 0.5;
    }
    glColor3d(r, g, b);
    glVertex3d(x, y, z);
  }
  glEnd();

  glEndList();
}

static void key(unsigned char k, int x, int y)
{
  int dirty = 0;
  switch (k)
  {
    case 'q':
    {
      double s = lorenz_get_s_parameter();
      s += 1;
      s = CLAMP_VALUE(s, 0, 100);
      lorenz_set_s_parameter(s);
      dirty = 1;
    }
    break;
    case 'a':
    {
      double s = lorenz_get_s_parameter();
      s -= 1;
      s = CLAMP_VALUE(s, 0, 100);
      lorenz_set_s_parameter(s);
      dirty = 1;
    }
    break;
    case 'w':
    {
      double b = lorenz_get_b_parameter();
      b += 1;
      b = CLAMP_VALUE(b, 0, 100);
      lorenz_set_b_parameter(b);
      dirty = 1;
    }
    break;
    case 's':
    {
      double b = lorenz_get_b_parameter();
      b -= 1;
      b = CLAMP_VALUE(b, 0, 100);
      lorenz_set_b_parameter(b);
      dirty = 1;
    }
    break;
    case 'e':
    {
      double r = lorenz_get_r_parameter();
      r += 1;
      r = CLAMP_VALUE(r, 0, 100);
      lorenz_set_r_parameter(r);
      dirty = 1;
    }
    break;
    case 'd':
    {
      double r = lorenz_get_r_parameter();
      r -= 1;
      r = CLAMP_VALUE(r, 0, 100);
      lorenz_set_r_parameter(r);
      dirty = 1;
    }
    break;
    case '1':
    {
      if (colorscheme != 1)
      {
        colorscheme = 1;
        dirty = 1;
      }
    };
    break;
    case '2':
    {
      if (colorscheme != 2)
      {
        colorscheme = 2;
        dirty = 1;
      }
    };
    break;
    case '3':
    {
      if (colorscheme != 3)
      {
        colorscheme = 3;
        dirty = 1;
      }
    };
    break;
    case ',':
    {
      distance -= 0.2f;
      distance = CLAMP_VALUE(distance, 15, 35);
    }
    break;
    case '.':
    {
      distance += 0.2f;
      distance = CLAMP_VALUE(distance, 15, 35);
    }
    break;
    default:
      return;
  }

  if (dirty)
  {
    lorenz_initialize();
    create_lorenz_attractor();
  }

  glutPostRedisplay();
}

static void special(int k, int x, int y)
{
  switch (k)
  {
    case GLUT_KEY_UP:
      pitch -= 5;
      break;
    case GLUT_KEY_DOWN:
      pitch += 5;
      break;
    case GLUT_KEY_LEFT:
      yaw -= 5;
      break;
    case GLUT_KEY_RIGHT:
      yaw += 5;
      break;
    default:
      return;
  }

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
  glutInitWindowSize(600, 600);
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
