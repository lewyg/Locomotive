#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "PrimitiveObject.h"
#include "Locomotive.h"
#include "Box.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Textures.h"

using namespace std;

static vector<glm::vec3> parts_position = {
	glm::vec3(0.0f, 0.0f, 0.0f),	//cabine
	glm::vec3(0.0f, 0.3f, 0.5f),	//cylinder
	glm::vec3(0.0f, 0.3f, 2.0f),	//cone
	glm::vec3(0.0f, -0.3f, 0.75f),	//base

	glm::vec3(-0.5f, -0.3f, 0.0f),	//wheels
	glm::vec3(-0.5f, -0.3f, 1.5f),
	glm::vec3(0.5f, -0.3f, 0.0f),
	glm::vec3(0.5f, -0.3f, 1.5f),

	glm::vec3(-0.725f, -0.35f, 0.75f),	//side_rods
	glm::vec3(0.725f, -0.35f, 0.75f),
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
	GLuint tex1 = loadTexture("img/steel.jpg");
	GLuint tex2 = loadTexture("img/container.jpg");
	this->parts.push_back(new Box(1.0f * size, 1.5f * size, 1.0 * size, tex1, 0, scalePosition(parts_position[0], size), start_rotation[0]));
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
	angl += getSpeed() / 20.0f * 0.3f;
	GLfloat newx;
	GLfloat newz;
	glm::vec3 pos1 = getPosition();
	newx = 20.0f * cos(angl);
	newz = 20.0f * sin(angl);
	setPosition(glm::vec3(newx, pos1.y, newz));
	glm::vec4 rot;
	for (int i = 0; i < wheels.size(); ++i)
	{
		rot = this->wheels[i]->getRotation();
		if (rot.x + rot.y + rot.z != 0.0f)
			rot.w += this->getSpeed();
		this->wheels[i]->setRotation(rot);
	}

	GLfloat angle = rot.w;
	GLfloat z_center, y_center, radius = 0.2f;
	GLfloat sinA = sin(angle);
	GLfloat cosA = cos(angle);
	glm::vec3 pos;
	for (int i = 0; i < side_rods.size(); ++i)
	{
		pos = this->side_rods[i]->getPosition();
		z_center = parts_position[8].z;
		y_center = parts_position[8].y;
		pos.z = -radius * cosA + z_center;
		pos.y = radius * sinA + y_center;
		this->side_rods[i]->setPosition(pos);
	}
}

void Locomotive::Draw(const glm::mat4& modelTrans, GLuint modelLoc, GLuint shader) const
{
	/*for each (PrimitiveObject* obj in parts)
	{
		obj->Draw(modelTrans, modelLoc, shader);
	}*/
	for (int i = 0; i < parts.size(); ++i)
		this->parts[i]->Draw(modelTrans, modelLoc, shader);
}

Locomotive::~Locomotive()
{
	for (int i = 0; i < parts.size(); ++i)
		delete(this->parts[i]);
}
