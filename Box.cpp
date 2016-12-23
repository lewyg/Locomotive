#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "PrimitiveObject.h"
#include "Box.h"

using namespace std;

static vector<GLfloat> unit_box = {
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

  -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,

  -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

  -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 0.0f
};

Box::Box(GLfloat size_x, GLfloat size_y, GLfloat size_z, GLuint texture, glm::vec3 position) : PrimitiveObject(texture, position)
{
	for (int i = 0; i < unit_box.size() / 5; ++i)
	{
		pushVertex(unit_box[i * 5] * size_x, unit_box[i * 5 + 1] * size_y, unit_box[i * 5 + 2] * size_z, unit_box[i * 5 + 3], unit_box[i * 5 + 4]);
	}

	bindVertices();
}
