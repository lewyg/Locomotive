#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#endif
#include "PrimitiveObject.h"
#include "PointLight.h"

PointLight::PointLight(PrimitiveObject * lam, glm::vec3 pos,
  glm::vec3 amb, glm::vec3 dif, glm::vec3 spe,
  GLfloat con, GLfloat lin, GLfloat qua)
    : lamp(lam), position(pos), ambient(amb), diffuse(dif), specular(spe), constant(con), linear(lin), quadratic(qua)
    {}

PointLight::~PointLight()
{
  delete(lamp);
}
