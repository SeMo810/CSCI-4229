#include "render/render_helper.h"
#include "render/ogl.h"

/* ============= File Variables =============== */
static GLint window = 0;
static TARGET_CAMERA camera;
/* ============================================ */

static void draw_gui()
{
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

  /* Object Rendering Here */

  draw_gui();

  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

static void idle()
{
  glutPostRedisplay();
}

static void key(unsigned char k, int x, int y)
{
  switch (k)
  {
    case '=':
      camera.distance -= 0.15;
      break;
    case '-':
      camera.distance += 0.15;
      break;
    /* Reset the camera */
    case '0':
      camera.distance = 15.0;
      camera.yaw = PI_O4;
      camera.pitch = PI_O4;
      break;
    default:
      return;
  }

  glutPostRedisplay();
}

static void special(int key, int x, int y)
{
  switch (key)
  {
    case GLUT_KEY_UP:
      camera.pitch += 0.03;
      camera.pitch = CLAMPVAL(camera.pitch, -PI_O2 + 0.0001, PI_O2 - 0.0001);
      break;
    case GLUT_KEY_DOWN:
      camera.pitch -= 0.03;
      camera.pitch = CLAMPVAL(camera.pitch, -PI_O2 + 0.0001, PI_O2 - 0.0001);
      break;
    case GLUT_KEY_LEFT:
      camera.yaw -= 0.03;
      break;
    case GLUT_KEY_RIGHT:
      camera.yaw += 0.03;
      break;
    default:
      return;
  }

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

  camera = (TARGET_CAMERA){ .target = (VEC3){ 0, 0, 0 }, .yaw = PI_O4, .pitch = PI_O4, .distance = 15.0 };
}

static void shutdown()
{

}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(600, 600);
  window = glutCreateWindow("3D Scene");
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
