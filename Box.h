#pragma once

class Box : public PrimitiveObject
{
public:
	Box(GLfloat size_x, GLfloat size_y, GLfloat size_z,
		GLuint textureDiffuse, GLuint textureSpecular,
		glm::vec3 position, glm::vec4 rotation);
};
