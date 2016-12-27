#pragma once
#include<iostream>
class Locomotive
{
	GLfloat speed;

	glm::vec3 position;

	std::vector<PrimitiveObject*> parts;
	std::vector<PrimitiveObject*> wheels;
	std::vector<PrimitiveObject*> side_rods;

public:
	GLfloat angl;
	Locomotive(GLfloat size);
	~Locomotive();
	void Draw(const glm::mat4&, GLuint modelLoc, GLuint shader) const;
	void Action();

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
		this->speed = speed > 2.0f ? 2.0f : speed;
	}

	void setPosition(glm::vec3 position)
	{
		this->position = position;
	}
};
