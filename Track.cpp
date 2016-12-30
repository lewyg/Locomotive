#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "Track.h"


static std::vector<GLfloat> pos = {
  0.0, 0.0, 0.0,
  1.0, 0.0, 0.0,
  2.0, 0.0, 0.0,
  3.0, 0.0, 0.0,
  3.0, 0.0, 1.0,
  3.0, 0.0, 2.0,
  3.0, 0.0, 3.0,
  2.0, 0.0, 3.0,
  1.0, 0.0, 3.0,
  0.0, 0.0, 2.0,
  0.0, 0.0, 1.0,
};

Track::Track()
{
  act = 0;
  for (int i = 0; i < pos.size(); i += 3)
  {
    positions.push_back(glm::vec3(pos[i], pos[i + 1], pos[i + 2]));
  }
}

glm::vec4 Track::getRotation()
{
  glm::vec4 result;
  return result;
}

GLfloat getAngle(glm::vec3 p1, glm::vec3 p2)
{
  GLfloat result;
  if ((p2.z - p1.z) == 0)
    result = 1.0;
  else
    result = (p2.x - p1.x) / (p2.z - p1.z);
  return result;
}

GLfloat getDistance(glm::vec3 p1, glm::vec3 p2)
{
  GLfloat result;

  result = sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
  return result;
}

int sgn(GLfloat val) {
    return (0 < val) - (val < 0);
}

glm::vec3 Track::nextPosition(glm::vec3 p, GLfloat speed)
{
  glm::vec3 result;
  int n = (act + 1) % positions.size();
  result = p;
  glm::vec3 next = positions[n];
  GLfloat factor = speed / getDistance(p, next);

  result.x += (next.x - p.x) * factor;
  result.z += (next.z - p.z) * factor;

  if (getDistance(p, positions[act]) > getDistance(p, next))
    act = n;

  return result;
}
