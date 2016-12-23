#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#include "PrimitiveObject.h"

using namespace std;

void PrimitiveObject::bindVertices()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * coord.size(), &(coord[0]), GL_STATIC_DRAW);

	// vertex geometry data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// vertex texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PrimitiveObject::Draw(const glm::mat4& modelTrans, GLuint modelLoc, GLuint shader) const
{
	glm::mat4 modelTrans2 = glm::translate(modelTrans, position);
	if (rotation.w != 0.0f)
		modelTrans2 = glm::rotate(modelTrans2, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));

	if (glGetUniformLocation(shader, "ourTexture") != texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shader, "ourTexture"), 0);
	}

	glBindVertexArray(VAO);

	// no internal transformations for now
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTrans2));

	glDrawArrays(GL_TRIANGLES, 0, (GLuint)coord.size());

	// all is drawn - unbind vertex array
	glBindVertexArray(0);
}

void PrimitiveObject::pushVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat tx, GLfloat ty)
{
	coord.push_back(x);
	coord.push_back(y);
	coord.push_back(z);
	coord.push_back(tx);
	coord.push_back(ty);
}

PrimitiveObject::~PrimitiveObject()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
