#pragma once
#include <vector>
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <Camera.h>

using namespace std;
class Node
{
public:
	Node(glm::vec3 position);
	//Position of the node
	glm::vec3 position;
	Node * child;
	static Node * end;
	static Node * secondEnd;
	void setPosition(glm::vec3 position);
};
class Head : public Node
{
public:
	Head(glm::vec3 position);
	Camera snakeCam = glm::vec3(0.0f, 0.0f, 3.0f);
	void moveSnake(float deltaTime);
};
class Apple
{
public:
	Apple();
	glm::vec3 position;
	void(*effect)(); // The Effect That is called
	void effectGrow();
	void effectLightShow();
	void effectDark();
	void effectExtraBig();
};