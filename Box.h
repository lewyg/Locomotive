#pragma once

class Box : public PrimitiveObject
{
public:
	Box(GLfloat size_x, GLfloat size_y, GLfloat size_z, GLuint texture, glm::vec3 position);
};
