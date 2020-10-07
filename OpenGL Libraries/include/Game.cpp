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
#include <ctime>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
const int NUMB_PTLIGHT = 4;

GLFWwindow * setUpGLFW();
Node * Node::end;
Node * Node::secondEnd;
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

void processInput(GLFWwindow * window);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);

float deltaTime, lastFrame;
bool flip = false;
double lastx, lasty;
bool firstMouse = true, firstLoop = true;

/* INITIALIZE HEAD */
Head head;
int main()
{
	/*          SETUP           */
	srand(time(0));
	glm::vec3 lightPositions[] = { glm::vec3(10.0f, 10.0f, -10.0f), glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(-10.0f, 10.0f, -10.0f), glm::vec3(-10.0f, 10.0f, 10.0f) };
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
	Model sphere("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/Attempt Two/NodeV2.obj");
	Model map("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/Attempt Two/Map.obj");
	/*        MATRIX          */
	glm::mat4 snakeModel;
	snakeModel = glm::scale(snakeModel, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 view = cam.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 200.0f);
	glm::mat4 projectionMap = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 200.0f);
	/*        SHADERS AND UNIFORMS        */
	Shader lightingShader("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/lightingShader.vs", "C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/lightingShader.fs");
	Shader noLightingShader("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/noLighting.vs", "C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/noLighting.fs");
	Shader appleShader("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/appleShader.vs", "C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/appleShader.fs");
	lightingShader.use();
	Shader * currentShader = &lightingShader;
	for (int i = 0; i < 2; i++)
	{
		currentShader->setBool("dark", GL_FALSE);
		currentShader->setMatrix4("model", snakeModel);
		currentShader->setMatrix4("view", view);
		currentShader->setMatrix4("projection", projection);
		currentShader->setVec3("viewPos", head.snakeCam.Position);
		for (int i = 0; i < NUMB_PTLIGHT; i++)
		{
			currentShader->setVec3(("pointLight[" + std::to_string(i) + "].position").c_str(), lightPositions[i]);
			currentShader->setVec3(("pointLight[" + std::to_string(i) + "].ambient").c_str(), glm::vec3(0.1f));
			currentShader->setVec3(("pointLight[" + std::to_string(i) + "].diffuse").c_str(), glm::vec3(1.0f));
			currentShader->setVec3(("pointLight[" + std::to_string(i) + "].specular").c_str(), glm::vec3(1.0f));
			currentShader->setFloat(("pointLight[" + std::to_string(i) + "].constant").c_str(), 1.0f);
			currentShader->setFloat(("pointLight[" + std::to_string(i) + "].linear").c_str(), 0.09f);
			currentShader->setFloat(("pointLight[" + std::to_string(i) + "].quadratic").c_str(), 0.032f);
		}
		currentShader = &appleShader;
		appleShader.use();
	}
	noLightingShader.use();
	noLightingShader.setMatrix4("projection", projection);
	/*        INITIALIZE SNAKE      */
	for (float i = 1; i < 3; i++)
	{
		new Node(glm::vec3(0.0f, 0.0f, i));
	}
	/*        INITIALIZE APPLE        */
	Apple apple;
	/*        RENDER LOOP       */
	while (!glfwWindowShouldClose(window))
	{
		lightingShader.use();
		deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();
		if (!firstLoop)
		{
			head.moveSnake(deltaTime);
		}
		if (glm::length(head.position - apple.position) <= 1)
		{
			(apple.*(apple.effect))();
			std::cout << "Apple" << std::endl;
		}
		Node * currentNode = head.child->child;
		bool gameOver = false;
		while (currentNode != nullptr)
		{
			if (glm::length(head.position - currentNode->position) <= 1)
			{
				gameOver = true;
			}
			currentNode = currentNode->child;
		}
		if (gameOver)
		{
			break;
		} 
		else if (50 - head.position.x < 0.5f || -50 - head.position.x > -0.5f || 50 - head.position.y < 0.5f || -50 - head.position.y > -0.5f || 50 - head.position.z < 0.5f || -50 - head.position.z > -0.5f)
		{
			break;
		}
		processInput(window);
		std::cout << head.snakeCam.Pitch << std::endl;
		lightingShader.use();
		if (fmod(head.snakeCam.Pitch, 360) > 90 && fmod(head.snakeCam.Pitch, 360) < 270 || fmod(head.snakeCam.Pitch, 360) < -90 && fmod(head.snakeCam.Pitch, 360) > -270)
		{
			flip = true;
			lightingShader.setMatrix4("transform", glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(0, 0, -1)));
			appleShader.use();
			appleShader.setMatrix4("transform", glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(0, 0, -1)));
		}
		else
		{
			flip = false;
			lightingShader.setMatrix4("transform", glm::mat4());
			appleShader.use();
			appleShader.setMatrix4("transform", glm::mat4());
		}
		lightingShader.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		view = glm::mat4();
		view = head.snakeCam.GetViewMatrix();
		//view = cam.GetViewMatrix();
		lightingShader.setMatrix4("view", view);
		lightingShader.setVec3("viewPos", head.snakeCam.Position);
		//lightingShader.setMatrix4("projection", projection);
		snakeModel = glm::mat4();
		currentNode = &head;
		while (currentNode != nullptr)
		{
			lightingShader.use();
			snakeModel = glm::mat4();
			snakeModel = glm::translate(snakeModel, currentNode->position);
			snakeModel = glm::scale(snakeModel, glm::vec3(0.5f, 0.5f, 0.5f));
			if (currentNode != &head)
			{
				lightingShader.setMatrix4("model", snakeModel);
				sphere.Draw(lightingShader);
			}
			currentNode = currentNode->child;
		}
		glm::mat4 modelApple;
		modelApple = glm::translate(modelApple, apple.position);
		modelApple = glm::scale(modelApple, glm::vec3(0.5f, 0.5f, 0.5f));
		appleShader.use();
		appleShader.setMatrix4("model", modelApple);
		appleShader.setMatrix4("view", view);
		appleShader.setVec3("viewPos", head.snakeCam.Position);
		sphere.Draw(appleShader);
		noLightingShader.use();
		noLightingShader.setMatrix4("model", glm::mat4());
		noLightingShader.setMatrix4("view", view);
		if (flip)
			noLightingShader.setMatrix4("transform", glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(0, 0, -1)));
		else
			noLightingShader.setMatrix4("transform", glm::mat4());
		map.Draw(noLightingShader);
		glfwPollEvents();
		glfwSwapBuffers(window);
		firstLoop = false;
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
	float xoffset;
	if (flip)
	{
		xoffset = lastx - xpos;
	}
	else
	{
		xoffset = xpos - lastx;
	}
	float yoffset = lasty - ypos;
	lastx = xpos;
	lasty = ypos;

	head.snakeCam.ProcessMouseMovement(xoffset, yoffset, false);
}