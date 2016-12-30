#pragma once

class Rail
{
	GLfloat size;

	std::vector<PrimitiveObject*> parts;
public:
	Rail(GLfloat size);
	~Rail();
	void Draw(const glm::mat4&, GLuint modelLoc, GLuint shader) const;
};
