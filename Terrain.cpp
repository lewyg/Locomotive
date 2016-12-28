#ifndef GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif
#include "PrimitiveObject.h"
#include "Terrain.h"

std::vector<char> readHeightMap(const char *file, int &width, int &height);

Terrain::Terrain(const char * file, GLuint tex)
  : PrimitiveObject(tex, 0, glm::vec3(0.0, 0.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 0.0))
{
  std::vector<char> pixels = readHeightMap(file, w, l);

  hs = new float*[l];
  for(int i = 0; i < l; i++)
  {
    hs[i] = new float[w];
  }

  normals = new glm::vec3*[l];
  for(int i = 0; i < l; i++)
  {
    normals[i] = new glm::vec3[w];
  }

  for(int y = 0; y < l; y++) {
      for(int x = 0; x < w; x++) {
          unsigned char color =
              (unsigned char)pixels[3 * (y * w + x)];
          float h = HEIGHT_MAP * ((color / 255.0f) - 0.5f);
          this->setHeight(x, y, h);
      }
  }
  //delete pixels;

  position = glm::vec3(-w / 2, 0.0, -l / 2);

  computedNormals = false;
  this->computeNormals();

  for(int z = 1; z < this->length() - 1; z++) {
		for(int x = 0; x < this->width() - 1; x++) {
      pushTerrainVertex(x, z);
      pushTerrainVertex(x, z - 1);
      pushTerrainVertex(x + 1, z);

      pushTerrainVertex(x, z);
      pushTerrainVertex(x + 1, z + 1);
      pushTerrainVertex(x + 1, z);
		}
	}

  bindVertices();
}

Terrain::~Terrain() {
  for(int i = 0; i < l; i++) {
    delete[] hs[i];
  }
  delete[] hs;

  for(int i = 0; i < l; i++) {
    delete[] normals[i];
  }
  delete[] normals;
}

GLfloat lengthV(glm::vec3 v)
{
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

glm::vec3 cross(glm::vec3 Left, glm::vec3 Right)
{
    glm::vec3 Result;
    Result.x = Left.y * Right.z - Left.z * Right.y;
    Result.y = Left.z * Right.x - Left.x * Right.z;
    Result.z = Left.x * Right.y - Left.y * Right.x;
    return Result;
}

glm::vec3 normalize(glm::vec3 Vec)
{
  GLfloat length = sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z);
  if(length == 0.0f)
  {
      return Vec;
  }
  else
  {
      float Factor = 1.0f / length;
      return glm::vec3(Vec.x * Factor, Vec.y * Factor, Vec.z * Factor);
  }
}

void Terrain::pushTerrainVertex(GLuint x, GLuint z)
{
  float factor = 4.0f;
  glm::vec3 normal = this->getNormal(x, z);
  pushVertex(
    x, this->getHeight(x, z), z,
    normal.x, normal.y, normal.z,
    (float)(x) / this->width() * factor, (float)(z) / this->length() * factor
  );
}

void Terrain::computeNormals() {
  if (computedNormals) {
    return;
  }

  //Compute the rough version of the normals
  glm::vec3** normals2 = new glm::vec3*[l];
  for(int i = 0; i < l; i++) {
    normals2[i] = new glm::vec3[w];
  }

  for(int z = 0; z < l; z++)
  {
    for(int x = 0; x < w; x++)
    {
      glm::vec3 sum(0.0f, 0.0f, 0.0f);

      glm::vec3 out;
      if (z > 0)
      {
        out = glm::vec3(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
      }
      glm::vec3 in;
      if (z < l - 1)
      {
        in = glm::vec3(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
      }
      glm::vec3 left;
      if (x > 0)
      {
        left = glm::vec3(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
      }
      glm::vec3 right;
      if (x < w - 1)
      {
        right = glm::vec3(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
      }

      if (x > 0 && z > 0)
      {
        sum += normalize(cross(out, left));
      }
      if (x > 0 && z < l - 1)
      {
        sum += normalize(cross(left, in));
      }
      if (x < w - 1 && z < l - 1)
      {
        sum += normalize(cross(in, right));
      }
      if (x < w - 1 && z > 0)
      {
        sum += normalize(cross(right, out));
      }

      normals2[z][x] = sum;
    }
  }

  //Smooth out the normals
  const float FALLOUT_RATIO = 0.5f;
  for(int z = 0; z < l; z++) {
    for(int x = 0; x < w; x++) {
      glm::vec3 sum = normals2[z][x];

      if (x > 0) {
        sum += normals2[z][x - 1] * FALLOUT_RATIO;
      }
      if (x < w - 1) {
        sum += normals2[z][x + 1] * FALLOUT_RATIO;
      }
      if (z > 0) {
        sum += normals2[z - 1][x] * FALLOUT_RATIO;
      }
      if (z < l - 1) {
        sum += normals2[z + 1][x] * FALLOUT_RATIO;
      }

      if (lengthV(sum) == 0) {
        sum = glm::vec3(0.0f, 1.0f, 0.0f);
      }
      normals[z][x] = sum;
    }
  }

  for(int i = 0; i < l; i++) {
    delete[] normals2[i];
  }
  delete[] normals2;

  computedNormals = true;
}

std::vector<char> readHeightMap(const char *file, int &width, int &height)
{
    static size_t HEADER_SIZE = 54;

    std::ifstream bmp;
    bmp.open(file, std::ifstream::binary);

    std::vector<char>  header(HEADER_SIZE);
    bmp.read(header.data(), HEADER_SIZE);

    int dataOffset = *reinterpret_cast<int *>(&header[10]);
    width = *reinterpret_cast<int *>(&header[18]);
    height = *reinterpret_cast<int *>(&header[22]);

    std::vector<char> img(dataOffset - HEADER_SIZE);
    bmp.read(img.data(), img.size());

    int dataSize = ((width * 3 + 3) & (~3)) * height;
    img.resize(dataSize);
    bmp.read(img.data(), img.size());
    bmp.close();

    return img;
}
