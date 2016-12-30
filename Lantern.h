#pragma once

class Lantern
{
	GLfloat size;

	std::vector<PrimitiveObject*> parts;
public:
  glm::vec3 position;
	Lantern(GLfloat size, glm::vec3 position);
	~Lantern();
	void Draw(const glm::mat4&, GLuint modelLoc, GLuint shader) const;
};
