#pragma once

class Cone : public PrimitiveObject
{
public:
	Cone(GLfloat height, GLfloat radius, GLuint n_sides, GLuint texture, glm::vec3 position);
};
