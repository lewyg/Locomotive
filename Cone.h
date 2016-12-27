#pragma once

class Cone : public PrimitiveObject
{
public:
	Cone(GLfloat height, GLfloat radius, GLuint n_sides,
		GLuint textureDiffuse, GLuint textureSpecular,
		glm::vec3 position, glm::vec4 rotation);
};
