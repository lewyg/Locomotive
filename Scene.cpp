#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#include "PrimitiveObject.h"
#include "Box.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Camera.h"
#include "Shader.h"
#include "Textures.h"
#include "Track.h"
#include "PointLight.h"
#include "Tree.h"
#include "Lantern.h"
#include "Locomotive.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Rail.h"
#include "Scene.h"

// Positions of the point lights
glm::vec3 lightPositions[] = {
    glm::vec3( 9.0f, 5.0f,  8.0f),
    glm::vec3( 0.0f, 0.1f,  0.0f),
};

glm::vec3 ambientLight(0.05f, 0.05f, 0.05f);
glm::vec3 dirLight(-1.0f, -0.7f, 0.8f);

Scene::Scene(Camera * cam)
{
  objectShader = new Shader("shader.vs", "shader.frag");
	lampShader = new Shader("lamp.vs", "lamp.frag");
  skyboxShader = new Shader("skybox.vs", "skybox.frag");

  skybox = new Skybox();

  terrain = new Terrain("img/heightmap.bmp", loadTexture("img/grass.jpg"));
  locomotive = new Locomotive(1.0f);

  camera = cam;
  rail = new Rail(1.0f);

  float x, y, z, s;
  float ss;
  Tree * tree;
  for (int i = 0; i < 400; ++i)
  {
    s = rand() % 25 + 8;
    ss = (float)s / 10.0;
    x = rand() % MAP_WIDTH;
    z = rand() % MAP_HEIGHT;
    y = terrain->getHeight(x, z);
    if (y < -0.2 && y > -0.4)
    {
      --i;
      continue;
    }

    tree = new Tree(ss, glm::vec3(x - 50, y - 0.15, z - 50));
    this->trees.push_back(tree);
  }

  float a;
  Lantern * lantern;
  PointLight * pl;
  glm::vec3 posLight;
  int r = TRACK_SIZE - 2;
  for (int i = 0; i < 2; ++i)
  {
    a = i * M_PI;
    posLight = glm::vec3(r * cos(a), -0.3, r * sin(a));
    lantern = new Lantern(1.0f, posLight);
    this->lanterns.push_back(lantern);

    pl = new PointLight(
      new Cylinder(0.3f, 0.1f, 6, 0, 0, posLight + glm::vec3(0.0, 2.2, 0.0), glm::vec4(1.0f, 0.0f, 0.0f, - M_PI / 2)),
      posLight + glm::vec3(0.0, 2.2, 0.0),
      ambientLight,
      glm::vec3(1.3f, 1.3f, 1.3f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      1.0f, 0.14f, 0.07f
    );
    lights.push_back(pl);
  }

  night = false;
}

void Scene::Draw()
{
  glm::mat4 view = camera->GetViewMatrix(); //glm::mat4(glm::mat3(camera->GetViewMatrix()));
  glm::mat4 projection = camera->GetProjection(800, 600);

  skyboxShader->Use();
  glDepthMask(GL_FALSE);// Remember to turn depth writing off
  glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  skybox->Draw(skyboxShader->Program);
  glDepthMask(GL_TRUE);

  view = camera->GetViewMatrix();
  projection = camera->GetProjection(800, 600);

  objectShader->Use();

  // Use cooresponding shader when setting uniforms/drawing objects
  GLint viewPosLoc = glGetUniformLocation(objectShader->Program, "viewPos");
  glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);
  // Set material properties
  glUniform1f(glGetUniformLocation(objectShader->Program, "material.shininess"), 32.0f);

  float difflight = (night ? 0.05 : 0.4);
  // Directional light
  glUniform3f(glGetUniformLocation(objectShader->Program, "dirLight.direction"), dirLight.x, dirLight.y, dirLight.z);
  glUniform3f(glGetUniformLocation(objectShader->Program, "dirLight.ambient"), ambientLight.x, ambientLight.y, ambientLight.z);
  glUniform3f(glGetUniformLocation(objectShader->Program, "dirLight.diffuse"), difflight, difflight, difflight);
  glUniform3f(glGetUniformLocation(objectShader->Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

  // Point lights
  glUniform1i(glGetUniformLocation(objectShader->Program, "nr_lights"), lights.size());
  for (int i = 0; i < lights.size(); ++i)
  {
    std::string is = std::to_string(i);
    glUniform3f(glGetUniformLocation(objectShader->Program, ("pointLights[" + is + "].position").c_str()), lights[i]->position.x, lights[i]->position.y, lights[i]->position.z);
    glUniform3f(glGetUniformLocation(objectShader->Program, ("pointLights[" + is + "].ambient").c_str()), ambientLight.x, ambientLight.y, ambientLight.z);
    glUniform3f(glGetUniformLocation(objectShader->Program, ("pointLights[" + is + "].diffuse").c_str()), lights[i]->diffuse.x, lights[i]->diffuse.y, lights[i]->diffuse.z);
    glUniform3f(glGetUniformLocation(objectShader->Program, ("pointLights[" + is + "].specular").c_str()), lights[i]->specular.x, lights[i]->specular.y, lights[i]->specular.z);
    glUniform1f(glGetUniformLocation(objectShader->Program, ("pointLights[" + is + "].constant").c_str()), lights[i]->constant);
    glUniform1f(glGetUniformLocation(objectShader->Program, ("pointLights[" + is + "].linear").c_str()), lights[i]->linear);
    glUniform1f(glGetUniformLocation(objectShader->Program, ("pointLights[" + is + "].quadratic").c_str()), lights[i]->quadratic);
  }

  // Get the uniform locations
  GLint modelLoc = glGetUniformLocation(objectShader->Program, "model");
  GLint viewLoc = glGetUniformLocation(objectShader->Program, "view");
  GLint projLoc = glGetUniformLocation(objectShader->Program, "projection");

  // Pass the matrices to the shader
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 transTerrain;
  terrain->Draw(transTerrain, modelLoc, objectShader->Program);

  GLfloat an, newx, newz;
  //glm::mat4 transRail;
  for (int a = 0; a < 240; ++a)
  {
    an = a * 1.5 * M_PI / 180.0;
    newx = TRACK_SIZE * cos(an);
  	newz = TRACK_SIZE * sin(an);
    glm::mat4 transRail;
    transRail = glm::translate(transRail, glm::vec3(newx, 0.0, newz));
    transRail = glm::rotate(transRail, -an, glm::vec3(0.0, 1.0, 0.0));

    rail->Draw(transRail, modelLoc, objectShader->Program);
  }

  glm::mat4 transLocomotive;
  glm::vec4 rotationLocomotive = locomotive->getRotation();
  transLocomotive = glm::translate(transLocomotive, locomotive->getPosition());
  transLocomotive = glm::rotate(transLocomotive, -rotationLocomotive.w, glm::vec3(rotationLocomotive.x, rotationLocomotive.y, rotationLocomotive.z));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transLocomotive));
  locomotive->Draw(transLocomotive, modelLoc, objectShader->Program);

  for (int i = 0; i < trees.size(); ++i)
  {
    glm::mat4 transTree;
    transTree = glm::translate(transTree, trees[i]->position);

    trees[i]->Draw(transTree, modelLoc, objectShader->Program);
  }

  for (int i = 0; i < lanterns.size(); ++i)
  {
    glm::mat4 transLantern;
    transLantern = glm::translate(transLantern, lanterns[i]->position);

    lanterns[i]->Draw(transLantern, modelLoc, objectShader->Program);
  }

  // Lamps
  lampShader->Use();
  // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
  modelLoc = glGetUniformLocation(lampShader->Program, "model");
  viewLoc  = glGetUniformLocation(lampShader->Program, "view");
  projLoc  = glGetUniformLocation(lampShader->Program, "projection");
  // Set matrices
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
  for (int i = 0; i < lights.size(); ++i)
  {
    glm::mat4 transLamp;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transLamp));
    lights[i]->lamp->Draw(transLamp, modelLoc, lampShader->Program);
  }
}

void Scene::checkCameraPosition()
{
  if (camera->Position.x < -49.3) camera->Position.x = -49.3;
  if (camera->Position.x > 49.3) camera->Position.x = 49.3;

  if (camera->Position.z < -49.3) camera->Position.z = -49.3;
  if (camera->Position.z > 49.3) camera->Position.z = 49.3;

  if (camera->Position.y > 49.3) camera->Position.y = 49.3;
  GLfloat y = terrain->getHeight((int)camera->Position.x + 50, (int)camera->Position.z + 50);
  if (camera->Position.x < 49)
    y = std::max(y, terrain->getHeight((int)camera->Position.x + 51, (int)camera->Position.z + 50));
  if (camera->Position.z < 49)
    y = std::max(y, terrain->getHeight((int)camera->Position.x + 50, (int)camera->Position.z + 51));
  if (camera->Position.z < 49 && camera->Position.x < 49)
    y = std::max(y, terrain->getHeight((int)camera->Position.x + 51, (int)camera->Position.z + 51));
  if (camera->Position.y < y + 0.6) camera->Position.y = y + 0.6;
}

void Scene::Action()
{
  locomotive->Action();

  //Camera
  if(camera->cameraMode == 1)
  {
    GLfloat angleCamera = locomotive->getAngle();
    camera->Position = glm::vec3(locomotive->getPosition() +
    glm::vec3(-5.0f * cos(angleCamera), 3.0f, -5.0f * sin(angleCamera)));
    camera->Yaw += locomotive->getAngleCamera();
    camera->updateCameraVectors();
  }
  else if (camera->cameraMode == 2)
  {
    GLfloat angleCamera = locomotive->getAngle() + M_PI / 2;
    camera->Position = glm::vec3(locomotive->getPosition() +
    glm::vec3(-5.0f * cos(angleCamera), 5.0f, -5.0f * sin(angleCamera)));
    camera->Yaw += locomotive->getAngleCamera();
    camera->updateCameraVectors();
  }
  checkCameraPosition();
}

void Scene::KeyHandler(bool keyUp, bool keyDown, GLfloat deltaTime)
{
  if (keyUp)
		locomotive->setSpeed(locomotive->getSpeed() + 0.1f * deltaTime);
	if (keyDown)
		locomotive->setSpeed(locomotive->getSpeed() - 0.1f * deltaTime);
}
