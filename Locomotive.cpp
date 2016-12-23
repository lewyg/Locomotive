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

static vector<glm::vec3> parts_position= {
	glm::vec3(0.0f, 0.0f, -0.5f),
	glm::vec3(0.0f, 0.0f, 0.0f)
};

glm::vec3 scalePosition(glm::vec3 pos, GLfloat size)
{
	glm::vec3 new_pos = glm::vec3(pos.r * size, pos.g * size, pos.b * size);
	return new_pos;
}


Locomotive::Locomotive(GLfloat size)
{
	GLuint tex1 = loadTexture("img/container.jpg");
	this->parts.push_back(new Cube(1.0f * size, tex1, scalePosition(parts_position[0], size)));
	this->parts.push_back(new Cone(1.0f * size,0.5f * size, 10, tex1, scalePosition(parts_position[1], size)));
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

}
