#pragma once

class Cube : public Box
{
public:
	Cube(GLfloat size,
		GLuint textureDiffuse, GLuint textureSpecular, 
		glm::vec3 position, glm::vec4 rotation);
};
