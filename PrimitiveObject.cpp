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
	this->rotateVerticesAroundAxis();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * coord.size(), &(coord[0]), GL_STATIC_DRAW);

	// vertex geometry data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// vertex normal coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//vertex texture coordinates
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PrimitiveObject::Draw(const glm::mat4& modelTrans, GLuint modelLoc, GLuint shader) const
{
	glm::mat4 modelTrans2 = glm::translate(modelTrans, position);
	if (rotation.w != 0.0f)
		modelTrans2 = glm::rotate(modelTrans2, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));

	//if (glGetUniformLocation(shader, "ourTexture") != texture)
	//{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureDiffuse);
		glUniform1i(glGetUniformLocation(shader, "material.diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureSpecular);
		glUniform1i(glGetUniformLocation(shader, "material.specular"), 1);
	//}

	glBindVertexArray(VAO);

	// no internal transformations for now
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTrans2));

	glDrawArrays(GL_TRIANGLES, 0, (GLuint)coord.size());

	// all is drawn - unbind vertex array
	glBindVertexArray(0);
}

void PrimitiveObject::pushVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat tx, GLfloat ty)
{
	coord.push_back(x);
	coord.push_back(y);
	coord.push_back(z);
	coord.push_back(nx);
	coord.push_back(ny);
	coord.push_back(nz);
	coord.push_back(tx);
	coord.push_back(ty);
}

void PrimitiveObject::rotateVerticesAroundAxis()
{
	glm::vec4 axis = this->getRotation();
	if (axis.x + axis.y + axis.z == 1.0f && axis.w != 0.0f)
	{
		GLfloat angle = axis.w;
		GLfloat sinA = sin(angle);
		GLfloat cosA = cos(angle);

		GLfloat x, y, z;
		for (int i = 0; i < coord.size(); i += 8)
		{
				x = this->coord[i];
				y = this->coord[i + 1];
				z = this->coord[i + 2];

				this->coord[i] = axis.x ? x :
					(axis.y ? cosA * x + sinA * z :
					cosA * x - sinA * y);

				this->coord[i + 1] = axis.y ? y :
					(axis.z ? cosA * y + sinA * x:
					cosA * y - sinA * z);

				this->coord[i + 2] = axis.z ? z :
					(axis.x ? cosA * z + sinA * y :
					cosA * z - sinA * x);

				//i += 3;

					x = this->coord[i + 3];
					y = this->coord[i + 4];
					z = this->coord[i + 5];

					this->coord[i + 3] = axis.x ? x :
						(axis.y ? cosA * x + sinA * z :
						cosA * x - sinA * y);

					this->coord[i + 4] = axis.y ? y :
						(axis.z ? cosA * y + sinA * x:
						cosA * y - sinA * z);

					this->coord[i + 5] = axis.z ? z :
						(axis.x ? cosA * z + sinA * y :
						cosA * z - sinA * x);
		}
	}
		this->setRotation(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

PrimitiveObject::~PrimitiveObject()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
