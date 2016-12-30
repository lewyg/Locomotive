#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "PrimitiveObject.h"
#include "Track.h"
#include "Locomotive.h"
#include "Box.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Textures.h"

using namespace std;

static vector<glm::vec3> parts_position = {
	glm::vec3(0.0f, 0.6f + WHEEL_SIZE, 0.0f - Z_OFFSET),	//cabine
	glm::vec3(0.0f, 0.6f + WHEEL_SIZE, 0.5f - Z_OFFSET),	//cylinder
	glm::vec3(0.0f, 0.6f + WHEEL_SIZE, 2.0f - Z_OFFSET),	//cone
	glm::vec3(0.0f, 0.0f + WHEEL_SIZE, 0.75f - Z_OFFSET),	//base

	glm::vec3(-0.5f, WHEEL_SIZE, 0.0f - Z_OFFSET),	//wheels
	glm::vec3(-0.5f, WHEEL_SIZE, 1.5f - Z_OFFSET),
	glm::vec3(0.5f, WHEEL_SIZE, 0.0f - Z_OFFSET),
	glm::vec3(0.5f, WHEEL_SIZE, 1.5f - Z_OFFSET),

	glm::vec3(-0.725f, -0.05f + WHEEL_SIZE, 0.75f - Z_OFFSET),	//side_rods
	glm::vec3(0.725f, -0.05f + WHEEL_SIZE, 0.75f - Z_OFFSET),
};

static vector<glm::vec4> start_rotation = {
	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), //nothing 0
	glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), //X axis 1
	glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), //Y axis 2
	glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), //Z axis 3

	glm::vec4(0.0f, 1.0f, 0.0f, -M_PI / 2.0f), //4
	glm::vec4(0.0f, 1.0f, 0.0f, M_PI / 2.0f), //5
};

glm::vec3 scalePosition(glm::vec3 pos, GLfloat size)
{
	glm::vec3 new_pos = glm::vec3(pos.r * size, pos.g * size, pos.b * size);
	return new_pos;
}


Locomotive::Locomotive(GLfloat size)
{
	this->size = size;
	this->setRotation(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	GLuint tex1 = loadTexture("img/steel.png");
	GLuint tex2 = loadTexture("img/wood.png");
	this->parts.push_back(new Box(1.0f * size, 1.2f * size, 1.0 * size, tex1, 0, scalePosition(parts_position[0], size), start_rotation[0]));
	this->parts.push_back(new Cylinder(1.5f * size, 0.5f * size, 20, tex1, 0, scalePosition(parts_position[1], size), start_rotation[2]));
	this->parts.push_back(new Cone(0.5f * size, 0.5f * size, 20, tex1, 0, scalePosition(parts_position[2], size), start_rotation[2]));
	this->parts.push_back(new Box(1.0f * size, 0.6f * size, 2.5f * size, tex1, 0, scalePosition(parts_position[3], size), start_rotation[0]));

	//wheels
	PrimitiveObject * wheel = new Cylinder(0.2f * size, 0.3f * size, 20, tex2, 0, scalePosition(parts_position[4], size), start_rotation[4]);
	this->parts.push_back(wheel);
	wheel->setRotation(start_rotation[1]);
	this->wheels.push_back(wheel);
	wheel = new Cylinder(0.2f * size, 0.3f * size, 20, tex2, 0, scalePosition(parts_position[5], size), start_rotation[4]);
	this->parts.push_back(wheel);
	wheel->setRotation(start_rotation[1]);
	this->wheels.push_back(wheel);
	wheel = new Cylinder(0.2f * size, 0.3f * size, 20, tex2, 0, scalePosition(parts_position[6], size), start_rotation[5]);
	this->parts.push_back(wheel);
	wheel->setRotation(start_rotation[1]);
	this->wheels.push_back(wheel);
	wheel = new Cylinder(0.2f * size, 0.3f * size, 20, tex2, 0, scalePosition(parts_position[7], size), start_rotation[5]);
	this->parts.push_back(wheel);
	wheel->setRotation(start_rotation[1]);
	this->wheels.push_back(wheel);

	//side_rods
	PrimitiveObject * rod = new Box(0.05f * size, 0.1f * size, 1.7f * size, tex1, 0, scalePosition(parts_position[8], size), start_rotation[0]);
	this->parts.push_back(rod);
	this->side_rods.push_back(rod);
	rod = new Box(0.05f * size, 0.1f * size, 1.7f * size, tex1, 0, scalePosition(parts_position[9], size), start_rotation[0]);
	this->parts.push_back(rod);
	this->side_rods.push_back(rod);
}

void Locomotive::Action()
{
	glm::vec4 * rot = &(this->rotation);
	rot->w += getSpeed() / TRACK_SIZE * (WHEEL_SIZE * size);
	GLfloat newx;
	GLfloat newz;
	glm::vec3 pos = getPosition();
	newx = TRACK_SIZE * cos(rot->w);
	newz = TRACK_SIZE * sin(rot->w);
	setPosition(glm::vec3(newx, pos.y, newz));

	//setPosition(track->nextPosition(getPosition(), getSpeed()));

	glm::vec4 rotWheel;
	for (int i = 0; i < wheels.size(); ++i)
	{
		rotWheel = this->wheels[i]->getRotation();
		if (rotWheel.x + rotWheel.y + rotWheel.z != 0.0f)
			rotWheel.w += this->getSpeed();
		this->wheels[i]->setRotation(rotWheel);
	}
	GLfloat angleWheel = rotWheel.w;
	GLfloat z_center, y_center, radius = (WHEEL_SIZE - 0.1f) * size;
	GLfloat sinA = sin(angleWheel);
	GLfloat cosA = cos(angleWheel);
	glm::vec3 posRod;
	for (int i = 0; i < side_rods.size(); ++i)
	{
		posRod = this->side_rods[i]->getPosition();
		z_center = parts_position[8].z * size;
		y_center = parts_position[8].y * size;
		posRod.z = -radius * cosA + z_center;
		posRod.y = radius * sinA + y_center;
		this->side_rods[i]->setPosition(posRod);
	}
}

void Locomotive::Draw(const glm::mat4& modelTrans, GLuint modelLoc, GLuint shader) const
{
	for (int i = 0; i < parts.size(); ++i)
		this->parts[i]->Draw(modelTrans, modelLoc, shader);
}

Locomotive::~Locomotive()
{
	for (int i = 0; i < parts.size(); ++i)
		delete(this->parts[i]);
}
