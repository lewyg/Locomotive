#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#endif
#include "PrimitiveObject.h"
#include "Cylinder.h"

using namespace std;

Cylinder::Cylinder(GLfloat height, GLfloat radius, GLuint sides, GLuint texture, glm::vec3 position) : PrimitiveObject(texture, position)
{
	GLfloat angle = 2 * M_PI / sides;
	GLfloat prev_x = 1.0f * radius;
	GLfloat prev_y = 0.0f;

	for (int i = 1; i <= sides; i++) {
		float x = cos(i * angle) * radius;
		float y = sin(i * angle) * radius;

		pushVertex(prev_x, prev_y, height, i % 2 ? 0 : 1, 0);
		pushVertex(x, y, height, i % 2 ? 1 : 0, 0);
		pushVertex(0, 0, height, 0.5, 1);

		pushVertex(prev_x, prev_y, 0, i % 2 ? 0 : 1, 0);
		pushVertex(x, y, 0, i % 2 ? 1 : 0, 0);
		pushVertex(0, 0, 0, 0.5, 1);

		pushVertex(prev_x, prev_y, height, 0, 0);
		pushVertex(x, y, height, 1, 0);
		pushVertex(prev_x, prev_y, 0, 0, 1);

		pushVertex(prev_x, prev_y, 0, 0, 1);
		pushVertex(x, y, 0, 1, 1);
		pushVertex(x, y, height, 1, 0);

		prev_x = x;
		prev_y = y;
	}

	bindVertices();
}
