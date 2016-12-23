#pragma once

#define M_PI 3.14159265358979323846

class PrimitiveObject
{
	GLuint VAO;
	GLuint VBO;

protected:
	glm::vec3 position;
	std::vector<GLfloat> coord;
	GLuint texture;

public:
	PrimitiveObject(GLuint tex, glm::vec3 pos)
		: texture(tex), position(pos) {}
	~PrimitiveObject();
	void bindVertices();
	void Draw(const glm::mat4&, GLuint modelLoc, GLuint shader) const;
	void pushVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat tx, GLfloat ty);

	glm::vec3 getPosition()
	{
		return position;
	}
	void setPosition(glm::vec3 position)
	{
		this->position = position;
	}
};
