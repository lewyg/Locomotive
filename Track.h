#pragma once

class Track
{
public:
  std::vector<glm::vec3> positions;

  int act;

	Track();

  glm::vec4 getRotation();

  glm::vec3 nextPosition(glm::vec3 p, GLfloat speed);
};
