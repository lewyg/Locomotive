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
#include "Camera.h"
#include "Shader.h"
#include "Textures.h"
#include "PointLight.h"
#include "Locomotive.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Scene.h"


// Positions of the point lights
glm::vec3 lightPositions[] = {
    glm::vec3( 9.0f, 5.0f,  8.0f),
    glm::vec3( 0.0f, 0.1f,  0.0f),
};

glm::vec3 ambientLight(0.05f, 0.05f, 0.05f);
glm::vec3 dirLight(-1.0f, -0.5f, 0.45f);

Scene::Scene(Camera * cam)
{
  objectShader = new Shader("shader.vs", "shader.frag");
	lampShader = new Shader("lamp.vs", "lamp.frag");
  skyboxShader = new Shader("skybox.vs", "skybox.frag");

  skybox = new Skybox();

  PointLight * pl = new PointLight(
    new Box(0.2f, 0.3f, 0.2f, 0, 0, lightPositions[0], glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)),
    lightPositions[0],
    ambientLight,
    glm::vec3(0.8f, 0.8f, 0.8f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f, 0.09f, 0.032f
  );
  lights.push_back(pl);

  PointLight * pl2 = new PointLight(
    new Box(0.2f, 0.3f, 0.2f, 0, 0, lightPositions[1], glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)),
    lightPositions[1],
    ambientLight,
    glm::vec3(0.8f, 0.8f, 0.8f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f, 0.09f, 0.032f
  );
  lights.push_back(pl2);

  terrain = new Terrain("img/heightmap3.bmp", loadTexture("img/skybox/bottom.png"));
  locomotive = new Locomotive(1.0f);
  camera = cam;
}

void Scene::Draw()
{
  glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
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

  // Directional light
  glUniform3f(glGetUniformLocation(objectShader->Program, "dirLight.direction"), dirLight.x, dirLight.y, dirLight.z);
  glUniform3f(glGetUniformLocation(objectShader->Program, "dirLight.ambient"), ambientLight.x, ambientLight.y, ambientLight.z);
  glUniform3f(glGetUniformLocation(objectShader->Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
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

  // SpotLight
  /*glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
  glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
  glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
  glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
  glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
  glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
  glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09);
  glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032);
  glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
  glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));*/

  // Get the uniform locations
  GLint modelLoc = glGetUniformLocation(objectShader->Program, "model");
  GLint viewLoc = glGetUniformLocation(objectShader->Program, "view");
  GLint projLoc = glGetUniformLocation(objectShader->Program, "projection");

  // Pass the matrices to the shader
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 transTerrain;
  terrain->Draw(transTerrain, modelLoc, objectShader->Program);

  glm::mat4 transLocomotive;
  glm::vec4 rotationLocomotive = locomotive->getRotation();
  transLocomotive = glm::translate(transLocomotive, locomotive->getPosition());
  transLocomotive = glm::rotate(transLocomotive, -rotationLocomotive.w, glm::vec3(rotationLocomotive.x, rotationLocomotive.y, rotationLocomotive.z));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transLocomotive));
  locomotive->Draw(transLocomotive, modelLoc, objectShader->Program);

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

void Scene::Action()
{
  locomotive->Action();

  //Camera modes
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
}

void Scene::KeyHandler(bool keyUp, bool keyDown, GLfloat deltaTime)
{
  if (keyUp)
		locomotive->setSpeed(locomotive->getSpeed() + 0.1f * deltaTime);
	if (keyDown)
		locomotive->setSpeed(locomotive->getSpeed() - 0.1f * deltaTime);
}
