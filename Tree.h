#pragma once

class Tree
{
	GLfloat size;
	std::vector<PrimitiveObject*> parts;
public:
  glm::vec3 position;
	Tree(GLfloat size, glm::vec3 position);
	~Tree();
	void Draw(const glm::mat4&, GLuint modelLoc, GLuint shader) const;
};
