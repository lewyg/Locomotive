#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "PrimitiveObject.h"
#include "Box.h"
#include "Cube.h"

using namespace std;

Cube::Cube(GLfloat size, GLuint texture, glm::vec3 position) : Box(size, size, size, texture, position)
{
	/*for (int i = 0; i < unit_cube.size() / 5; ++i)
	{
		pushVertex(unit_cube[i * 5] * size, unit_cube[i * 5 + 1] * size, unit_cube[i * 5 + 2] * size, unit_cube[i * 5 + 3], unit_cube[i * 5 + 4]);
	}

	bindVertices();*/
}
