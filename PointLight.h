#pragma once

class PointLight
{
public:
	PrimitiveObject * lamp;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	PointLight(PrimitiveObject * lam, glm::vec3 pos,
		glm::vec3 amb, glm::vec3 dif, glm::vec3 spe,
		GLfloat con, GLfloat lin, GLfloat qua);
	~PointLight();
};
