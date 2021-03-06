#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "PrimitiveObject.h"
#include "Box.h"
#include "Cube.h"

using namespace std;

Cube::Cube(GLfloat size,
	GLuint textureDiffuse, GLuint textureSpecular,
	glm::vec3 position, glm::vec4 rotation)
	: Box(size, size, size, textureDiffuse, textureSpecular, position, rotation)
{}
