#ifndef GLEW_STATIC
#define GLEW_STATIC
#define MAP_SIZE 50
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif
#include <iostream>
#include "Textures.h"
#include "Skybox.h"

GLfloat skyboxVertices[] = {
    // Positions
    -MAP_SIZE,  MAP_SIZE, -MAP_SIZE,
    -MAP_SIZE, -MAP_SIZE, -MAP_SIZE,
     MAP_SIZE, -MAP_SIZE, -MAP_SIZE,
     MAP_SIZE, -MAP_SIZE, -MAP_SIZE,
     MAP_SIZE,  MAP_SIZE, -MAP_SIZE,
    -MAP_SIZE,  MAP_SIZE, -MAP_SIZE,

    -MAP_SIZE, -MAP_SIZE,  MAP_SIZE,
    -MAP_SIZE, -MAP_SIZE, -MAP_SIZE,
    -MAP_SIZE,  MAP_SIZE, -MAP_SIZE,
    -MAP_SIZE,  MAP_SIZE, -MAP_SIZE,
    -MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
    -MAP_SIZE, -MAP_SIZE,  MAP_SIZE,

     MAP_SIZE, -MAP_SIZE, -MAP_SIZE,
     MAP_SIZE, -MAP_SIZE,  MAP_SIZE,
     MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
     MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
     MAP_SIZE,  MAP_SIZE, -MAP_SIZE,
     MAP_SIZE, -MAP_SIZE, -MAP_SIZE,

    -MAP_SIZE, -MAP_SIZE,  MAP_SIZE,
    -MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
     MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
     MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
     MAP_SIZE, -MAP_SIZE,  MAP_SIZE,
    -MAP_SIZE, -MAP_SIZE,  MAP_SIZE,

    -MAP_SIZE,  MAP_SIZE, -MAP_SIZE,
     MAP_SIZE,  MAP_SIZE, -MAP_SIZE,
     MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
     MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
    -MAP_SIZE,  MAP_SIZE,  MAP_SIZE,
    -MAP_SIZE,  MAP_SIZE, -MAP_SIZE,

    -MAP_SIZE, -MAP_SIZE, -MAP_SIZE,
    -MAP_SIZE, -MAP_SIZE,  MAP_SIZE,
     MAP_SIZE, -MAP_SIZE, -MAP_SIZE,
     MAP_SIZE, -MAP_SIZE, -MAP_SIZE,
    -MAP_SIZE, -MAP_SIZE,  MAP_SIZE,
     MAP_SIZE, -MAP_SIZE,  MAP_SIZE
};

Skybox::Skybox()
{
  std::vector<const GLchar*> faces;
  faces.push_back("img/skybox/right.png");
  faces.push_back("img/skybox/left.png");
  faces.push_back("img/skybox/top.png");
  faces.push_back("img/skybox/bottom.png");
  faces.push_back("img/skybox/back.png");
  faces.push_back("img/skybox/front.png");
  cubemapTexture = loadCubemap(faces);

  // Setup skybox VAO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Skybox::Draw(GLuint shader)
{
  // skybox cube
  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(shader, "skybox"), 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
