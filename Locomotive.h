#pragma once

class Locomotive
{
	GLfloat speed;
	glm::vec3 position;

	std::vector<PrimitiveObject*> parts;

public:
	Locomotive(GLfloat size);
	~Locomotive();
	void Draw(const glm::mat4&, GLuint modelLoc, GLuint shader) const;

	GLfloat getSpeed()
	{
		return speed;
	}
	glm::vec3 getPosition()
	{
		return position;
	}

	void setSpeed(GLfloat speed)
	{
		this->speed = speed;
	}

	void setPosition(glm::vec3 position)
	{
		this->position = position;
	}
};
