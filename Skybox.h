#pragma once

class Skybox
{

public:
  GLuint VAO;
  GLuint VBO;
	GLuint cubemapTexture;

	Skybox();
  void Draw(GLuint shader);
};
