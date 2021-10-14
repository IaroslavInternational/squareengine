#pragma once

#include "InteractableObject2D.h"
#include <vector>

class InteractableObject2DContainer
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	InteractableObject2DContainer(std::string dataPath);
public:
	void Translate(DirectX::XMFLOAT2 delta);
private:
	std::vector<InteractableObject2D> objects;
};

