#include <math.h>
#include <stdio.h>
#include "render/ogl.h"
#include "render/render_helper.h"

/* ============= File Variables =============== */
static GLint window = 0;
static TARGET_CAMERA tcamera;
static FPS_CAMERA fcamera;
static double cuberot = 0.0;
static double polyrot = 0.0;
static double spherot = 0.0;
static int lasttime = 0;
static int camera_type = 1;
static int window_x = 600, window_y = 600;
/* ============================================ */

static void draw_gui()
{
  glColor3d(1, 1, 1);
  VEC3 cfwd = cm_get_fps_camera_forward(fcamera);
  VEC3 cright = cm_get_fps_camera_right(fcamera);
  VEC3 cup = vec3_cross(cright, cfwd);
  rh_draw_window_text((VEC2){ 3, 3 }, "Camera Type: %s", camera_type == 1 ? "Perspective Target" : (camera_type == 2 ? "Orthographic Target" : "Perspective FPS"));
  rh_draw_window_text((VEC2){ 3, 25}, "Camera Position: (%f,%f,%f)", fcamera.position.x, fcamera.position.y, fcamera.position.z);
  rh_draw_window_text((VEC2){ 3, 50}, "Camera Forward: (%f,%f,%f)", cfwd.x, cfwd.y, cfwd.z);
  rh_draw_window_text((VEC2){ 3, 75}, "Camera Right: (%f,%f,%f)", cright.x, cright.y, cright.z);
  rh_draw_window_text((VEC2){ 3, 100}, "Camera Up: (%f,%f,%f)", cup.x, cup.y, cup.z);
}

static void apply_projection()
{
  float aspect = (float)window_x / (float)window_y;

  if (camera_type == 1 || camera_type == 3)
    cm_apply_perspective(45, aspect);
  else
    cm_apply_ortho(5, aspect);
}

static void draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  apply_projection();

  glPushMatrix();
  glLoadIdentity();

  if (camera_type == 1 || camera_type == 2)
    cm_apply_target_camera(tcamera);
  else
    cm_apply_fps_camera(fcamera);

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
  spherot = 45.0 * sin(time / 1000.0);

  lasttime = time;
  glutPostRedisplay();
}

static void key(unsigned char k, int x, int y)
{
  /* Change the camera type, if needed. */
  switch (k)
  {
    case 27: /* Escape Key. */
      glutDestroyWindow(window);
      exit(0);
      break;
    case '1':
      camera_type = 1;
      break;
    case '2':
      camera_type = 2;
      break;
    case '3':
      camera_type = 3;
      break;
    default:
      break;
  }

  if (camera_type == 1 || camera_type == 2)
    cm_target_camera_key(&tcamera, k);
  else
    cm_fps_camera_key(&fcamera, k);

  glutPostRedisplay();
}

static void special(int key, int x, int y)
{
  if (camera_type == 1 || camera_type == 2)
    cm_target_camera_special(&tcamera, key);
  else
    cm_fps_camera_special(&fcamera, key);

  glutPostRedisplay();
}

static void reshape(int w, int h)
{
  window_x = w;
  window_y = h;

  glViewport(0, 0, (GLint)w, (GLint)h);
  apply_projection();
  glMatrixMode(GL_MODELVIEW);
}

static void init(int argc, char **argv)
{
  glEnable(GL_DEPTH_TEST);

  tcamera = (TARGET_CAMERA){ .target = (VEC3){ 0, 0, 0 }, .yaw = PI_O4, .pitch = PI_O4, .distance = 15.0 };
  fcamera = (FPS_CAMERA){ .position = (VEC3){ 10, 10, 10 }, .yaw = PI_O4, .pitch = PI_O4 };
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
