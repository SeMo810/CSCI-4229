#include <math.h>
#include <stdio.h>
#include "render/render_helper.h"
#include "render/ogl.h"
#include "render/lighting.h"

/* ============= File Variables =============== */
static GLint window = 0;
static TARGET_CAMERA tcamera;
static LIGHTING_SETTINGS lsettings;
static CYLINDER_LIGHT light;
static double cuberot = 0.0;
static double polyrot = 0.0;
static double spherot = 0.0;
static int lasttime = 0;
static int lightPaused = 0;
static int shininess = 8;
/* ============================================ */

#define CLAMP01(val) ((val)<0?0:((val)>1?1:(val)))

static void draw_gui()
{
  glColor3d(1, 1, 1);
  VEC3 pos0 = lht_get_cylinder_light_position(light);
  rh_draw_window_text((VEC2){ 5, 125 }, "[Light]");
  rh_draw_window_text((VEC2){ 5, 110 }, "    Paused = %s", lightPaused ? "true" : "false");
  rh_draw_window_text((VEC2){ 5, 95 },  "    Position = (%.2f, %.2f, %.2f)", pos0.x, pos0.y, pos0.z);
  rh_draw_window_text((VEC2){ 5, 80 },  "    AmbientColor = (%1.2f, %1.2f, %1.2f)", light.ambientColor.x, light.ambientColor.y, light.ambientColor.z);
  rh_draw_window_text((VEC2){ 5, 65 },  "    DiffuseColor = (%1.2f, %1.2f, %1.2f)", light.diffuseColor.x, light.diffuseColor.y, light.diffuseColor.z);
  rh_draw_window_text((VEC2){ 5, 50 },  "    SpecularColor = (%1.2f, %1.2f, %1.2f)", light.specularColor.x, light.specularColor.y, light.specularColor.z);
  rh_draw_window_text((VEC2){ 5, 35 },  "    AmbientIntensity = %1.3f", light.ambientIntensity);
  rh_draw_window_text((VEC2){ 5, 20 },  "    DiffuseIntensity = %1.3f", light.diffuseIntensity);
  rh_draw_window_text((VEC2){ 5,  5 },  "    SpecularIntensity = %1.3f", light.specularIntensity);

  rh_draw_window_text((VEC2){ 5, 185 }, "[Material]");
  rh_draw_window_text((VEC2){ 5, 170 }, "    Shininess = %d", shininess);
  rh_draw_window_text((VEC2){ 5, 155 }, "    Smooth = %s", lsettings.smooth ? "true" : "false");
}

static void draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glLoadIdentity();

  cm_apply_target_camera(tcamera);

  lht_draw_cylinder_light(light);
  lht_prepare_lighting(lsettings);
  lht_set_light(0, light);

  float shiny[] = { pow(2.0, shininess) };
  float spec[] = { 1.0, 1.0, 1.0, 1.0 };
  glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
  glMaterialfv(GL_FRONT, GL_SPECULAR, spec);

  /* +/-X Cubes */
  rh_draw_cube((VEC3){ 3, 0, 0 }, (VEC3){ 1, 1, 1 }, (VEC3){ 0, cuberot, 0 });
  rh_draw_cube((VEC3){ -3, 0, 0 }, (VEC3){ 1, 1, 1 }, (VEC3){ 0, cuberot, 0 });
  /* +/-Z Cubes */
  rh_draw_cube((VEC3){ 0, 0, 3 }, (VEC3){ 1.25, 1.25, 1.25 }, (VEC3){ 0, -cuberot, 0 });
  rh_draw_cube((VEC3){ 0, 0, -3 }, (VEC3){ 1.25, 1.25, 1.25 }, (VEC3){ 0, -cuberot, 0 });
  /* +/-Y Cubes */
  rh_draw_cube((VEC3){ 0, 5, 0 }, (VEC3){ 1.5, 1.5, 1.5 }, (VEC3){ 0, 0, 0 });
  rh_draw_cube((VEC3){ 0, -5, 0 }, (VEC3){ 1.5, 1.5, 1.5 }, (VEC3){ 0, 0, 0 });

  /* Sphere stack of varing sizes and qualities */
  rh_draw_sphere((VEC3){ 0, 0, 0 }, (VEC3){ 1, 1, 1 }, (VEC3){ spherot, 0, 0 }, 3);
  rh_draw_sphere((VEC3){ 0, -1, 0 }, (VEC3){ .5, .5, .5 }, (VEC3){ -spherot, 0, 0 }, 2);
  rh_draw_sphere((VEC3){ 0, -1.5, 0 }, (VEC3){ .25, .25, .25 }, (VEC3){ spherot, 0, 0 }, 1);
  rh_draw_sphere((VEC3){ 0, 1, 0 }, (VEC3){ .5, .5, .5 }, (VEC3){ -spherot, 0, 0 }, 2);
  rh_draw_sphere((VEC3){ 0, 1.5, 0 }, (VEC3){ .25, .25, .25 }, (VEC3){ spherot, 0, 0 }, 1);

  /* Orbiting Objects */
  rh_draw_sphere((VEC3){ 3 * sin(cuberot * 0.005), 2, 3 * cos(cuberot * 0.005) }, (VEC3){ 1, 1, 1 }, (VEC3){ 0, 0, 0 }, 3);
  rh_draw_cube((VEC3){ 3 * sin(cuberot * 0.005), 2, 3 * cos(cuberot * 0.005) }, (VEC3){ .25, 2, .25 }, (VEC3){ 0, 0, 0 });
  rh_draw_sphere((VEC3){ 3 * sin(-cuberot * 0.005), -2, 3 * cos(-cuberot * 0.005) }, (VEC3){ 1, 1, 1 }, (VEC3){ 0, 0, 0 }, 3);
  rh_draw_cube((VEC3){ 3 * sin(-cuberot * 0.005), -2, 3 * cos(-cuberot * 0.005) }, (VEC3){ .25, 2, .25 }, (VEC3){ 0, 0, 0 });

  glDisable(GL_LIGHTING);

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

  if (!lightPaused)
    light.angle += (dtime / 2.0);

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
    case 'l': /* Enable/Disable automatic light movement. */
      lightPaused = 1 - lightPaused;
      break;
    case '[': /* Move light ccw in manual mode. */
      if (lightPaused)
        light.angle += .05;
      break;
    case ']': /* Move light cw in manual mode. */
      if (lightPaused)
        light.angle -= .05;
      break;
    case ',': /* Move light down. */
      light.height -= 0.1;
      break;
    case '.': /* Move light up. */
      light.height += 0.1;
      break;
    case ';': /* Move light in. */
      light.distance -= 0.1;
      if (light.distance < 0)
        light.distance = 0;
      break;
    case '\'': /* Move light out. */
      light.distance += 0.1;
      break;
    case 'a': /* Decrease ambient intensity. */
      light.ambientIntensity = CLAMP01(light.ambientIntensity - 0.05);
      break;
    case 'A': /* Increase ambient intensity. */
      light.ambientIntensity = CLAMP01(light.ambientIntensity + 0.05);
      break;
    case 's': /* Decrease specular intensity. */
      light.specularIntensity = CLAMP01(light.specularIntensity - 0.05);
      break;
    case 'S': /* Increase specular intensity. */
      light.specularIntensity = CLAMP01(light.specularIntensity + 0.05);
      break;
    case 'd': /* Decrease diffuse intensity. */
      light.diffuseIntensity = CLAMP01(light.diffuseIntensity - 0.05);
      break;
    case 'D': /* Increase diffuse intensity. */
      light.diffuseIntensity = CLAMP01(light.diffuseIntensity + 0.05);
      break;
    case '1': /* Change ambient light to white. */
      light.ambientColor = (VEC3){ 1, 1, 1 };
      break;
    case '2': /* Change ambient light to red. */
      light.ambientColor = (VEC3){ 1, 0.5, 0.5 };
      break;
    case '3': /* Change ambient light to green. */
      light.ambientColor = (VEC3){ 0.5, 1, 0.5 };
      break;
    case '4': /* Change ambient light to blue. */
      light.ambientColor = (VEC3){ 0.5, 0.5, 1 };
      break;
    case '5': /* Change diffuse light to white. */
      light.diffuseColor = (VEC3){ 1, 1, 1 };
      light.specularColor = (VEC3){ 1, 1, 1 };
      break;
    case '6': /* Change diffuse light to red. */
      light.diffuseColor = (VEC3){ 1, 0.5, 0.5 };
      light.specularColor = (VEC3){ 1, 0.5, 0.5 };
      break;
    case '7': /* Change diffuse light to green. */
      light.diffuseColor = (VEC3){ 0.5, 1, 0.5 };
      light.specularColor = (VEC3){ 0.5, 1, 0.5 };
      break;
    case '8': /* Change diffuse light to blue. */
      light.diffuseColor = (VEC3){ 0.5, 0.5, 1 };
      light.specularColor = (VEC3){ 0.5, 0.5, 1 };
      break;
    case 'f': /* Decrease material shininess */
      shininess = CLAMPVAL(shininess - 1, 0, 8);
      break;
    case 'F': /* Increase material shininess */
      shininess = CLAMPVAL(shininess + 1, 0, 8);
      break;
    case 'm': /* Change the shading model. */
      lsettings.smooth = 1 - lsettings.smooth;
      break;
    default:
      break;
  }

  cm_target_camera_key(&tcamera, k);

  glutPostRedisplay();
}

static void special(int key, int x, int y)
{
  cm_target_camera_special(&tcamera, key);

  glutPostRedisplay();
}

static void reshape(int w, int h)
{
  glViewport(0, 0, (GLint)w, (GLint)h);
  float aspect = (float)w / (float)h;
  cm_apply_perspective(45, aspect);
  glMatrixMode(GL_MODELVIEW);
}

static void init(int argc, char **argv)
{
  glEnable(GL_DEPTH_TEST);

  tcamera = (TARGET_CAMERA){ .target = (VEC3){ 0, 0, 0 }, .yaw = PI_O4, .pitch = PI_O4, .distance = 15.0 };
  lsettings = lht_create_default_lighting_settings();
  light = lht_create_default_cylinder_light();
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
  window = glutCreateWindow("HW5 - Sean Moss");
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
