#ifndef LIGHTING_HPP_
#define LIGHTING_HPP_

#include "../config.hpp"
#include "../math.hpp"

// Lighting namespace
namespace LIGHT
{

struct Material
{
  float ambientIntensity;
  float diffuseIntensity;
  float specularIntensity;
  math::Vec3f ambientColor;
  math::Vec3f diffuseColor;
  math::Vec3f specularColor;

  Material() :
    ambientIntensity(0), diffuseIntensity(0), specularIntensity(0),
    ambientColor(1, 1, 1), diffuseColor(1, 1, 1), specularColor(1, 1, 1)
  { }
};

extern void initialize_lighting();

extern void enable_lighting();
extern void disable_lighting();

extern void set_ambient_lighting(float inten);
extern void set_diffuse_lighting(float inten);
extern void set_specular_lighting(float inten);

extern float get_ambient_lighting();
extern float get_diffuse_lighting();
extern float get_specular_lighting();

extern void set_ambient_color(const math::Vec3f& color);
extern void set_diffuse_color(const math::Vec3f& color);
extern void set_specular_color(const math::Vec3f& color);

extern math::Vec3f get_ambient_color();
extern math::Vec3f get_diffuse_color();
extern math::Vec3f get_specular_color();

extern void applyMaterial(const Material& material);
extern void update_lighting();

extern void set_time_of_day(float time);
extern void progress_time_of_day(float time);
extern float get_time_of_day();

}

#endif // LIGHTING_HPP_
