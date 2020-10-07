#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <Camera.h>
#include <Snake.h>
#include <iostream>
using namespace std;
SnakeCam::SnakeCam(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :Camera(position, up, yaw, pitch) {}
void SnakeCam::move(float deltaTime)
{
	ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
}
Node::Node(glm::vec3 position) : position(position) 
{
	if(end != nullptr)
		end->child = this;
	secondEnd = end;
	parent = end;
	end = this;
}
Node::Node()
{
	end->child = this;
	position = (end->position - secondEnd->position) + end->position;
	secondEnd = end;
	parent = end;
	end = this;
}
void Node::setPosition(glm::vec3 position)
{
	if (child != nullptr)
	{
		
		child->setPosition(this->position);
	}
	this->position = position;
}
Head::Head(glm::vec3 position) : Node(position)
{
	snakeCam.Position = position;
}
void Head::moveSnake(float deltaTime)
{

	snakeCam.move(deltaTime);
	//std::cout << position.x << " " << position.y << " " << position.z << std::endl << child->position.x << " " << child->position.y << " " << child->position.z << std::endl << child->child->position.x << " " << child->child->position.y << " " << child->child->position.z << std::endl;
	position = snakeCam.Position;
	if (child != nullptr)
		child->path.push_back(position);
		child->workTask(deltaTime);
}
void Node::workTask(float deltaTime)
{
	float mag = SPEED * deltaTime;
	vector<glm::vec3> steps;
	//float zeroLength = vectorLength(path[0])
	 if (path.size() == 0)
	 {
		 path.push_back(parent->position);
	 }
	glm::vec3 normalDirection = glm::normalize(path[0] - position);
	glm::vec3 displace = normalDirection * (mag);
	glm::vec3 stepSum;
	float extra;
	int i = 0;
	float totalLength = 0;
	bool greater = false;
	for(; glm::length(displace) - glm::length(path[i] - (position + stepSum)) > 0; i++)
	{
		greater = true;
		if (path.size() == i+1)
		{
			path.push_back(parent->position);
		}
		extra = glm::length(displace) - glm::length(path[i] - (position + stepSum));
		stepSum += path[i] - (position + stepSum);
		totalLength += glm::length(path[i] - (position + stepSum));
		displace = path[i + 1] - path[i];
		displace = glm::normalize(displace);
		displace *= extra;
		std::cout << glm::length(displace) << " " << extra << std::endl;
	}

	bool didReach = true;
	if (i == 0)
	{
		stepSum = displace;
		totalLength = glm::length(displace);
		didReach = false;
	}
	else if (i > 0)
	{
		stepSum += displace;
		totalLength += glm::length(displace);
	}
	if (totalLength != mag)
	{
		cout << totalLength << " " << mag << this << std::endl;
	}
	position += stepSum;
	for (int j = 0; j < i; j++)
	{
		if(child != nullptr)
			child->path.push_back(path[j]);
	}
	if(didReach)
	{
		path.erase(path.begin(), path.begin() + i);
	}
	if(child != nullptr)
		child->workTask(deltaTime);
}