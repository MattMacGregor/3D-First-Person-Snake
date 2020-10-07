#pragma once
#include <Camera.h>

class MiniMapCamera : public Camera
{
public:
	MiniMapCamera(int playWidth = 100, int playHeight = 100, int playDepth = 100);
};