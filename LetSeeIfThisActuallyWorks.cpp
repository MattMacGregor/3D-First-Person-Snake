#include <Shader.h>
#include <Camera.h>
#include <Mesh.h>
#include <Model.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Snake.h>
#include <Apple.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

GLFWwindow * setUpGLFW();

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

void processInput(GLFWwindow * window);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);

float deltaTime, lastFrame;

double lastx, lasty;
bool firstMouse = true;
int main2()
{
	/*          SETUP           */
	GLFWwindow *window;
	window = setUpGLFW();

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	/*        LOAD MODELS             */
	Model sphere("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/Attempt Two/Node.obj");
	Model nano("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/Attempt Two/nanosuit.obj");
	/*        MATRIX          */
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 view = cam.GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	/*        SHADERS AND UNIFORMS        */
	Shader lightingShader("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/lightingShader.vs", "C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/lightingShader.fs");
	Shader noLightingShader("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/noLighting.vs", "C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/noLighting.fs");
	lightingShader.use();
	lightingShader.setBool("dark", GL_FALSE);
	lightingShader.setMatrix4("model", model);
	lightingShader.setMatrix4("view", view);
	lightingShader.setMatrix4("projection", projection);
	lightingShader.setVec3("viewPos", cam.Position);
	lightingShader.setVec3("pointLight.position", glm::vec3());
	lightingShader.setVec3("pointLight.ambient", glm::vec3(0.1f));
	lightingShader.setVec3("pointLight.diffuse", glm::vec3(1.0f));
	lightingShader.setVec3("pointLight.specular", glm::vec3(1.0f));
	lightingShader.setFloat("pointLight.constant", 1.0f);
	lightingShader.setFloat("pointLight.linear", 0.09f);
	lightingShader.setFloat("pointLight.quadratic", 0.032f);
	noLightingShader.use();
	noLightingShader.setMatrix4("model", model);
	noLightingShader.setMatrix4("view", view);
	noLightingShader.setMatrix4("projection", projection);
	/*        MINIMAP           */
	Camera minimap(glm::vec3(0.0f, 20.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, -90.f);
	/*        RENDER LOOP       */
	while (!glfwWindowShouldClose(window))
	{
		lightingShader.use();
		deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		view = glm::mat4();
		view = cam.GetViewMatrix();
		lightingShader.setMatrix4("view", view);
		lightingShader.setVec3("viewPos", cam.Position);
		model = glm::mat4();
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::translate(model, glm::vec3((float) sin(glfwGetTime()), 0.0f, -2.0f));
		model = glm::rotate(model,(float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 modelNano;
		modelNano = glm::translate(modelNano, glm::vec3((float)sin(glfwGetTime()), -4.0f, -8.0f));
		modelNano = glm::rotate(modelNano, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMatrix4("model", model);
		sphere.Draw(lightingShader);
		lightingShader.setMatrix4("model", modelNano);
		nano.Draw(lightingShader);
		noLightingShader.use();
		glm::mat4 minimapMat;
		minimapMat = glm::lookAt(minimap.Position, minimap.Position + minimap.Front, minimap.Up);
		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(-0.8f, -0.8f, -0.8f));
		transform = glm::scale(transform, glm::vec3(0.1f, 0.1f, 0.1f));
		noLightingShader.setMatrix4("transform", transform);
		noLightingShader.setMatrix4("view", minimapMat);
		noLightingShader.setMatrix4("model", model);
		sphere.Draw(noLightingShader);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return 0;
}
GLFWwindow * setUpGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Attempt Two", NULL, NULL);
	glfwMakeContextCurrent(window);
	return window;
}
void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
}
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastx = xpos;
		lasty = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastx;
	float yoffset = lasty - ypos;
	lastx = xpos;
	lasty = ypos;

	cam.ProcessMouseMovement(xoffset, yoffset, GL_TRUE);
}