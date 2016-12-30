#pragma once

#define MAP_WIDTH 100
#define MAP_HEIGHT 100
class Scene
{
  Shader * skyboxShader;
  Shader * objectShader;
  Shader * lampShader;

  std::vector<PointLight *> lights;
  std::vector<Lantern *> lanterns;
  std::vector<Tree *> trees;

  Locomotive * locomotive;
  Skybox * skybox;
  Camera * camera;
  Terrain * terrain;
  Rail * rail;
public:
  bool night;
	Scene(Camera * cam);
  void Draw();
  void Action();
  void KeyHandler(bool keyUp, bool keyDown, GLfloat deltaTime);
  void checkCameraPosition();
};
