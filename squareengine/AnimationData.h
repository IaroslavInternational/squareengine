#pragma once

#include <string>

struct AnimationData
{
	AnimationData(int pStart, int pEnd, int width, int height, int frames, float ft, std::string name)
	{
		this->pStart = pStart;
		this->pEnd = pEnd;
		this->width = width;
		this->height = height;
		this->frames = frames;
		this->ft = ft;
		this->name = name;
	}

	int pStart;
	int pEnd;
	int width;
	int height;
	int frames;
	float ft;
	std::string name;
};
