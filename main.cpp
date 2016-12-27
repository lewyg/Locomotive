#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "PrimitiveObject.h"
#include "Box.h"
#include "Locomotive.h"

#include "SOIL.h"
//#include "Textures.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLFWwindow* initGL();
GLuint loadTexture(const char * file);
GLuint loadCubemap(vector<const GLchar*> faces);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool cameraMode = false;
GLfloat anglecamera;

GLfloat x = 0;



GLfloat skyboxVertices[] = {
    // Positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

// Positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

Locomotive * loco;
Box * ground;
// The MAIN function, from here we start our application and run our Game loop
int main()
{
	GLFWwindow* window = initGL();

  vector<const GLchar*> faces;
  faces.push_back("img/skybox/right.jpg");
  faces.push_back("img/skybox/left.jpg");
  faces.push_back("img/skybox/top.jpg");
  faces.push_back("img/skybox/bottom.jpg");
  faces.push_back("img/skybox/back.jpg");
  faces.push_back("img/skybox/front.jpg");
  GLuint cubemapTexture = loadCubemap(faces);

	// Setup and compile our shaders
	Shader ourShader("shader.vs", "shader.frag");
	Shader lampShader("lamp.vs", "lamp.frag");
  Shader skyboxShader("skybox.vs", "skybox.frag");
  PrimitiveObject * p1 = new Box(0.2f, 0.2f, 0.2f, 0, 0, pointLightPositions[0], glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
  PrimitiveObject * p2 = new Box(0.2f, 0.2f, 0.2f, 0, 0, pointLightPositions[1], glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
  PrimitiveObject * p3 = new Box(0.2f, 0.2f, 0.2f, 0, 0, pointLightPositions[2], glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
  PrimitiveObject * p4 = new Box(0.2f, 0.2f, 0.2f, 0, 0, pointLightPositions[3], glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
  ground = new Box(500.0f, 0.1f, 500.0f, loadTexture("img/skybox/bottom.jpg"), 0, glm::vec3(0.0f, -0.7f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

  // Setup skybox VAO
  GLuint skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glBindVertexArray(0);

	loco = new Locomotive(1.0f);

	double a = 0, b = 0;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw skybox first
      glDepthMask(GL_FALSE);// Remember to turn depth writing off
      skyboxShader.Use();
      glm::mat4 view1 = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
      glm::mat4 projection1 = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
      glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view1));
      glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection1));
      // skybox cube
      glBindVertexArray(skyboxVAO);
      glActiveTexture(GL_TEXTURE0);
      glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthMask(GL_TRUE);

		ourShader.Use();

		// Use cooresponding shader when setting uniforms/drawing objects
    GLint viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPos");
    glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
    // Set material properties
    glUniform1f(glGetUniformLocation(ourShader.Program, "material.shininess"), 32.0f);
    // == ==========================
    // Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
    // the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
    // by defining light types as classes and set their values in there, or by using a more efficient uniform approach
    // by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
    // == ==========================
    // Directional light
    glUniform3f(glGetUniformLocation(ourShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
    // Point light 1
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].linear"), 0.7);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].quadratic"), 1.8);
    // Point light 2
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].linear"), 0.7);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].quadratic"), 1.8);
    // Point light 3
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].linear"), 0.7);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].quadratic"), 1.8);
    // Point light 4
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].linear"), 0.7);
    glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].quadratic"), 1.8);
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

    if (cameraMode)
    {
      anglecamera = loco->angl - M_PI / 4;
      camera.Position = glm::vec3(loco->getPosition() + glm::vec3(cos(anglecamera) * 4.0f, 3.0f, sin(anglecamera) * 4.0f));
      camera.Yaw += loco->getSpeed() / 20.0f * 0.3f * 180 / M_PI;
      camera.updateCameraVectors();
    }
		// Create camera transformation
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniform1f(glGetUniformLocation(ourShader.Program, "lightOff"), true);
    glm::mat4 trans2;
    ground->Draw(trans2, modelLoc, ourShader.Program);
    //glUniform1f(glGetUniformLocation(ourShader.Program, "lightOff"), false);

		loco->Action();
		a = (a + 0.001);
		b += loco->getSpeed() / 20.0f * 0.3f;
		glm::mat4 trans;
		trans = glm::translate(trans, loco->getPosition());
		GLfloat angle = -b;
		trans = glm::rotate(trans, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
		loco->Draw(trans, modelLoc, ourShader.Program);

    // Also draw the lamp object, again binding the appropriate shader
    lampShader.Use();
    // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
    modelLoc = glGetUniformLocation(lampShader.Program, "model");
    viewLoc  = glGetUniformLocation(lampShader.Program, "view");
    projLoc  = glGetUniformLocation(lampShader.Program, "projection");
    // Set matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		p1->Draw(trans, modelLoc, lampShader.Program);
    p2->Draw(trans, modelLoc, lampShader.Program);
    p3->Draw(trans, modelLoc, lampShader.Program);
    p4->Draw(trans, modelLoc, lampShader.Program);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

GLFWwindow* initGL()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	return window;
}




GLuint loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width,height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}


// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_UP])
		loco->setSpeed(loco->getSpeed() + 0.05f * deltaTime);
	if (keys[GLFW_KEY_DOWN])
		loco->setSpeed(loco->getSpeed() - 0.05f * deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
  if (key == GLFW_KEY_C && (action == GLFW_RELEASE))
    cameraMode = !cameraMode;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset * 0.5);
}
