#ifndef MATH_HPP_
#define MATH_HPP_

#include "config.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/transform.hpp"

namespace math
{

typedef glm::tvec2<unsigned char> Vec2ub;
typedef glm::tvec2<signed char> Vec2b;
typedef glm::tvec2<unsigned short> Vec2us;
typedef glm::tvec2<signed short> Vec2s;
typedef glm::tvec2<unsigned int> Vec2ui;
typedef glm::tvec2<signed int> Vec2i;
typedef glm::tvec2<float> Vec2f;
typedef glm::tvec2<double> Vec2d;

typedef glm::tvec3<unsigned char> Vec3ub;
typedef glm::tvec3<signed char> Vec3b;
typedef glm::tvec3<unsigned short> Vec3us;
typedef glm::tvec3<signed short> Vec3s;
typedef glm::tvec3<unsigned int> Vec3ui;
typedef glm::tvec3<signed int> Vec3i;
typedef glm::tvec3<float> Vec3f;
typedef glm::tvec3<double> Vec3d;

typedef glm::tvec4<unsigned char> Vec4ub;
typedef glm::tvec4<signed char> Vec4b;
typedef glm::tvec4<unsigned short> Vec4us;
typedef glm::tvec4<signed short> Vec4s;
typedef glm::tvec4<unsigned int> Vec4ui;
typedef glm::tvec4<signed int> Vec4i;
typedef glm::tvec4<float> Vec4f;
typedef glm::tvec4<double> Vec4d;

}

#endif // MATH_HPP_
