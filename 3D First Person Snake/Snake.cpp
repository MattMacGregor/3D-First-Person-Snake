#include <Model.h>
#include <Shader.h>
#include <C:\Users\Ian MacGregor\Documents\Visual Studio 2017\Projects\3D First Person Snake\3D First Person Snake\Snake.h>
#define PLAY_WIDTH 1000
#define PLAY_HEIGHT 1000
#define PLAY_DEPTH 1000
double lastx, lasty;
bool firstMouse = true;
void Head::moveSnake(float deltaTime)
{
	float velocity = snakeCam.MovementSpeed * deltaTime;
	if (child != nullptr)
	{
		child->setPosition(position);
	}
	position = snakeCam.Front * velocity;
}
Node::Node(glm::vec3 position)
{
	this->position = position;
}
void Node::setPosition(glm::vec3 position)
{
	this->position = position;
}
Head::Head(glm::vec3 position) : Node(position)
{}
Node * Node::end;
Node * Node::secondEnd;
Apple::Apple()
{
	this->position = glm::vec3((float)(rand() % PLAY_WIDTH), (float)(rand() % PLAY_HEIGHT), (float)(rand() % PLAY_DEPTH));
}
void Apple::effectGrow()
{
	glm::vec3 difference = Node::secondEnd->position - Node::end->position;
	new Node(difference + Node::end->position);
	this->position = glm::vec3((float)(rand() % PLAY_WIDTH), (float)(rand() % PLAY_HEIGHT), (float)(rand() % PLAY_DEPTH));
}
void Apple::effectDark()
{
	//Make Everything Dark except for flashlight
	this->effectGrow();
}
void Apple::effectLightShow()
{
	//Strobe Lights Everywhere
	this->effectGrow();
}
void Apple::effectExtraBig()
{
	//Scale all nodes by factor
	this->effectGrow();
}
void processInput(GLFWwindow * window);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);

double lastX, lastY;

float deltaTime, lastFrame;

Head head(glm::vec3(0.0f, 0.0f, 0.0f));

void main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * window = glfwCreateWindow(800, 800, "SNAKE BABY", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, 800, 800);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Model nodeModel("C:/Users/Ian MacGregor/Documents/Visual Studio 2017/Projects/3D First Person Snake/Node.obj");

	Shader shader("lightingShader.vs", "lightingShader.fs");

	glm::mat4 model;
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	shader.use();
	shader.setMatrix4("model", model);
	shader.setMatrix4("projection", projection);
	shader.setBool("dark", true);
	shader.setVec3("pointLight.position", glm::vec3(7.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight.constant", 1.0f);
	shader.setFloat("pointLight.linear", 0.09f);
	shader.setFloat("pointLight.quadratic", 0.032f);
	shader.setVec3("pointLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setVec3("pointLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	while (true)
	{
		glm::mat4 view = head.snakeCam.GetViewMatrix();
		shader.setVec3("viewPos", head.snakeCam.Position);
		shader.setMatrix4("view", view);
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		processInput(window);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		nodeModel.Draw(shader);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		head.snakeCam.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		head.snakeCam.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		head.snakeCam.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		head.snakeCam.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);

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
	double xoffset = xpos - lastx;
	double yoffset = lasty - ypos;
	lastx = xpos;
	lasty = ypos;

	head.snakeCam.ProcessMouseMovement((float)xoffset, (float)yoffset, GL_TRUE);
}