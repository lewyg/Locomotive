#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "PrimitiveObject.h"
#include "Box.h"
#include "Textures.h"
#include "Rail.h"

using namespace std;

static vector<glm::vec3> parts_position = {
	glm::vec3(0.0f, -0.3f, 0.0f),	//cabine
	glm::vec3(-0.6f, -0.1f, 0.0f),	//cylinder
	glm::vec3(0.6f, -0.1f, 0.0f),	//cone
};

static vector<glm::vec4> start_rotation = {
	glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), //nothing 0
	glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), //X axis 1
	glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), //Y axis 2
	glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), //Z axis 3

	glm::vec4(0.0f, 1.0f, 0.0f, -M_PI / 2.0f), //4
	glm::vec4(0.0f, 1.0f, 0.0f, M_PI / 2.0f), //5
};

glm::vec3 scalePosition(glm::vec3 pos, GLfloat size);

Rail::Rail(GLfloat size)
{
  this->size = size;
  GLuint tex1 = loadTexture("img/steel.png");
  GLuint tex_wood = loadTexture("img/wood.png");
  this->parts.push_back(new Box(1.8f * size, 0.2f * size, 0.3 * size, tex_wood, 0, scalePosition(parts_position[0], size), start_rotation[0]));
  this->parts.push_back(new Box(0.1f * size, 0.1f * size, 0.9 * size, tex1, 0, scalePosition(parts_position[1], size), start_rotation[0]));
  this->parts.push_back(new Box(0.1f * size, 0.1f * size, 0.9 * size, tex1, 0, scalePosition(parts_position[2], size), start_rotation[0]));
}

void Rail::Draw(const glm::mat4& modelTrans, GLuint modelLoc, GLuint shader) const
{
	for (int i = 0; i < parts.size(); ++i)
		this->parts[i]->Draw(modelTrans, modelLoc, shader);
}
