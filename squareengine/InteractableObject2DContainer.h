#pragma once

#include "InteractableObject2D.h"
#include <vector>

class InteractableObject2DContainer
{
public:
	friend class Layers;
public:
	InteractableObject2DContainer(std::string dataPath);
private:
	std::vector<InteractableObject2D> objects;
};

