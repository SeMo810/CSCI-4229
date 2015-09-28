#include <math.h>
#include <stdio.h>
#include "render/ogl.h"
#include "render/render_helper.h"

/* ============= File Variables =============== */
static GLint window = 0;
static TARGET_CAMERA camera;
static double cuberot = 0.0;
static double polyrot = 0.0;
static double spherot = 0.0;
static int lasttime = 0;
/* ============================================ */

static void draw_gui()
{
  glColor3d(1, 1, 1);
  rh_draw_window_text((VEC2){ 3, 3 }, "Pitch: %.3fpi rad", camera.pitch / PI);
  rh_draw_window_text((VEC2){ 170, 3 }, "Yaw: %.3fpi rad", camera.yaw / PI);
  rh_draw_window_text((VEC2){ 340, 3 }, "Distance: %.2f", camera.distance);
}

static void draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glLoadIdentity();

  rh_draw_world_axes(camera, (VEC3){ 5, 5, 5 });
  cm_apply_target_camera(camera);

  /* +/-X Cubes */
  rh_draw_cube((VEC3){ 3, 0, 0 }, (VEC3){ 1, 1, 1 }, (VEC3){ 0, cuberot, 0 });
  rh_draw_cube((VEC3){ -3, 0, 0 }, (VEC3){ 1, 1, 1 }, (VEC3){ 0, cuberot, 0 });
  /* +/-Z Cubes */
  rh_draw_cube((VEC3){ 0, 0, 3 }, (VEC3){ 1.25, 1.25, 1.25 }, (VEC3){ 0, -cuberot, 0 });
  rh_draw_cube((VEC3){ 0, 0, -3 }, (VEC3){ 1.25, 1.25, 1.25 }, (VEC3){ 0, -cuberot, 0 });

  /* Sphere stack of varing sizes and qualities */
  rh_draw_sphere((VEC3){ 0, 0, 0 }, (VEC3){ 1, 1, 1 }, (VEC3){ spherot, 0, 0 }, 3);
  rh_draw_sphere((VEC3){ 0, -1, 0 }, (VEC3){ .5, .5, .5 }, (VEC3){ -spherot, 0, 0 }, 2);
  rh_draw_sphere((VEC3){ 0, -1.5, 0 }, (VEC3){ .25, .25, .25 }, (VEC3){ spherot, 0, 0 }, 1);

  /* Polygon stack of various edge counts and rotations */
  rh_draw_extended_polygon((VEC3){ 0, 1, 0 }, (VEC3){ 1, .25, 1 }, (VEC3){ 0, polyrot, 0 }, 3);
  rh_draw_extended_polygon((VEC3){ 0, 1.75, 0 }, (VEC3){ 1, .25, 1 }, (VEC3){ 0, -polyrot, 0 }, 4);
  rh_draw_extended_polygon((VEC3){ 0, 2.5, 0 }, (VEC3){ 1, .25, 1 }, (VEC3){ 0, polyrot, 0 }, 5);
  rh_draw_extended_polygon((VEC3){ 0, 3.25, 0 }, (VEC3){ 1, .25, 1 }, (VEC3){ 0, -polyrot, 0 }, 7);
  rh_draw_extended_polygon((VEC3){ 0, 4, 0 }, (VEC3){ 1, .25, 1 }, (VEC3){ 0, polyrot, 0 }, 10);

  draw_gui();
  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

static void idle()
{
  int time = glutGet(GLUT_ELAPSED_TIME);
  double dtime = (time - lasttime) / 1000.0;

  cuberot += (dtime * 100);
  polyrot += (dtime * 50);
  spherot = 45.0*sin(time / 1000.0);

  lasttime = time;
  glutPostRedisplay();
}

static void key(unsigned char k, int x, int y)
{
  /* TODO: Change it so the camera has its own key function. */

  glutPostRedisplay();
}

static void special(int key, int x, int y)
{
  /* TODO: Change it so the camera has its own special key function. */

  glutPostRedisplay();
}

static void reshape(int w, int h)
{
  GLfloat aspect = (GLfloat)w / (GLfloat)h;

  glViewport(0, 0, (GLint)w, (GLint)h);
  cm_apply_perspective(45, aspect);
  glMatrixMode(GL_MODELVIEW);
}

static void init(int argc, char **argv)
{
  glEnable(GL_DEPTH_TEST);

  camera = (TARGET_CAMERA){ .target = (VEC3){ 0, 0, 0 }, .yaw = PI_O4, .pitch = PI_O4 + 0.00001, .distance = 15.0 };
}

static void shutdown()
{

}

int main(int argc, char **argv)
{
  fprintf(stdout, "Sean Moss");

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowPosition(100, 100);
  glutInitWindowSize(600, 600);
  window = glutCreateWindow("HW4 - Sean Moss");
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
