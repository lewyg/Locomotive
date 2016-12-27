#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#endif
#include "PrimitiveObject.h"
#include "Box.h"

using namespace std;

GLfloat vertices[] = {

    };

static vector<GLfloat> unit_box = {
  // Positions          // Normals           // Texture Coords
  -0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
   0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
   0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
   0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
  -0.5f,  1.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
  -0.5f,  0.0f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

  -0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
   0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
   0.5f,  1.0f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
   0.5f,  1.0f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
  -0.5f,  1.0f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
  -0.5f,  0.0f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

  -0.5f,  1.0f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
  -0.5f,  1.0f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
  -0.5f,  0.0f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  -0.5f,  0.0f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
  -0.5f,  0.0f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  -0.5f,  1.0f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

   0.5f,  1.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
   0.5f,  1.0f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
   0.5f,  0.0f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
   0.5f,  0.0f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
   0.5f,  0.0f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
   0.5f,  1.0f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

  -0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
   0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
   0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
   0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
  -0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

  -0.5f,  1.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
   0.5f,  1.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
   0.5f,  1.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
   0.5f,  1.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
  -0.5f,  1.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
  -0.5f,  1.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

Box::Box(GLfloat size_x, GLfloat size_y, GLfloat size_z,
  GLuint textureDiffuse, GLuint textureSpecular,
  glm::vec3 position, glm::vec4 rotation)
  : PrimitiveObject(textureDiffuse, textureSpecular, position, rotation)
{
	for (int i = 0; i < unit_box.size() / 8; ++i)
	{
		pushVertex(
      unit_box[i * 8] * size_x,
      unit_box[i * 8 + 1] * size_y,
      unit_box[i * 8 + 2] * size_z,
      unit_box[i * 8 + 3],
      unit_box[i * 8 + 4],
      unit_box[i * 8 + 5],
      unit_box[i * 8 + 6],
      unit_box[i * 8 + 7]);
	}

	bindVertices();
}
