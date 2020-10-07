#pragma once
#include <glm/glm.hpp>
#define PLAY_WIDTH 100
#define PLAY_HEIGHT 100
#define PLAY_DEPTH 100
class Apple
{
public:
	Apple();
	glm::vec3 position;
	void effectGrow();
	void (Apple::*effect)(); // The Effect That is called
};