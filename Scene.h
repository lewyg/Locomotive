#pragma once

class Scene
{
  Shader * skyboxShader;
  Shader * objectShader;
  Shader * lampShader;

  std::vector<PointLight *> lights;
  std::vector<PrimitiveObject *> objects;

  Locomotive * locomotive;
  Skybox * skybox;
  Camera * camera;
  Box * ground;

public:
	Scene(Camera * cam);
  void Draw();
  void Action();
  void KeyHandler(bool keyUp, bool keyDown, GLfloat deltaTime);
};
