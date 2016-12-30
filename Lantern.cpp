#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "PrimitiveObject.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Textures.h"
#include "Lantern.h"

using namespace std;

static vector<glm::vec3> parts_position = {
	glm::vec3(0.0f, -0.3f, 0.0f),
	glm::vec3(0.0f, 2.5f, 0.0f),
};

static vector<glm::vec4> start_rotation = {
	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), //nothing 0
	glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), //X axis 1
	glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), //Y axis 2
	glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), //Z axis 3

	glm::vec4(1.0f, 0.0f, 0.0f, -M_PI / 2.0f), //4
	glm::vec4(1.0f, 0.0f, 0.0f, M_PI / 2.0f), //5
};

glm::vec3 scalePosition(glm::vec3 pos, GLfloat size);

Lantern::Lantern(GLfloat size, glm::vec3 position)
{
  this->size = size;
  this->position = position;
  GLuint tex1 = loadTexture("img/steel.png");
  this->parts.push_back(new Cylinder(2.5f * size, 0.05f * size, 5, tex1, 0, scalePosition(parts_position[0], size), start_rotation[4]));
  this->parts.push_back(new Cone(0.1 * size, 0.1f * size, 5, tex1, 0, scalePosition(parts_position[1], size), start_rotation[4]));
}

void Lantern::Draw(const glm::mat4& modelTrans, GLuint modelLoc, GLuint shader) const
{
	for (int i = 0; i < parts.size(); ++i)
		this->parts[i]->Draw(modelTrans, modelLoc, shader);
}
