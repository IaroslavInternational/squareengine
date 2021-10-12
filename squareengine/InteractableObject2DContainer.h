#pragma once

#include "InteractableObject2D.h"
#include <vector>

class InteractableObject2DContainer
{
public:
	InteractableObject2DContainer();
private:
	std::vector<InteractableObject2D> objects;
};

