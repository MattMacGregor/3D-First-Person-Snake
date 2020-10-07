#pragma once
#include <glm\glm.hpp>
#include <Camera.h>
#include <sstream>
using namespace std;
class SnakeCam : public Camera
{
public:
	SnakeCam(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	void move(float deltaTime);
};
class Node
{
public:
	Node();
	Node(glm::vec3 position);
	//Position of the node
	glm::vec3 position;
	Node * child;
	Node * parent;
	static Node * end;
	static Node * secondEnd;
	void setPosition(glm::vec3 position);
	void workTask(float deltaTime);
	vector<glm::vec3> path;
};
class Head : public Node
{
public:
	Head(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	SnakeCam snakeCam = glm::vec3(0.0f, 0.0f, 0.0f);
	static const float SNAKE_SPEED;
	void moveSnake(float deltaTime);
};