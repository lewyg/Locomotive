#pragma once
#include<iostream>

#define TRACK_SIZE 32.0
#define WHEEL_SIZE 0.3
#define Z_OFFSET 0.7
class Locomotive
{
	GLfloat speed;
	GLfloat size;
	glm::vec3 position;
	glm::vec4 rotation;

	std::vector<PrimitiveObject*> parts;
	std::vector<PrimitiveObject*> wheels;
	std::vector<PrimitiveObject*> side_rods;

public:
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
	glm::vec4 getRotation()
	{
		return rotation;
	}
	GLfloat getAngle()
	{
		return rotation.w;
	}
	GLfloat getAngleCamera()
	{
		return getSpeed() / TRACK_SIZE * (WHEEL_SIZE * size) * 180.0 / M_PI;
	}

	void setSpeed(GLfloat speed)
	{
		this->speed = speed > 2.0f ? 2.0f : speed;
	}
	void setPosition(glm::vec3 position)
	{
		this->position = position;
	}
	void setRotation(glm::vec4 rotation)
	{
		this->rotation = rotation;
	}
};
