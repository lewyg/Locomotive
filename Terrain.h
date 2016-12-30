#pragma once

#define HEIGHT_MAP 20.0

class Terrain : public PrimitiveObject
{
	private:
		int w; //Width
		int l; //Length
		float** hs; //Heights
		glm::vec3** normals;
		bool computedNormals; //Whether normals is up-to-date
	public:
		Terrain(const char * file, GLuint tex);

		~Terrain();

		int width()
    {
			return w;
		}

		int length()
    {
			return l;
		}

		//Sets the height at (x, z) to y
		void setHeight(int x, int z, float y)
    {
			hs[z][x] = y;
			computedNormals = false;
		}

		//Returns the height at (x, z)
		float getHeight(int x, int z)
    {
			return hs[z][x];
		}

		//Computes the normals, if they haven't been computed yet
		void computeNormals();

		void pushTerrainVertex(GLuint x, GLuint z);

		//Returns the normal at (x, z)
		glm::vec3 getNormal(int x, int z) {
			if (!computedNormals) {
				computeNormals();
			}
			return normals[z][x];
		}
};
