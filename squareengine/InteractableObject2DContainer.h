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
	void DeleteObjectAt(size_t k);
	void DeleteObjectAt(std::vector<std::unique_ptr<InteractableObject2D>>::iterator it);
private:
	std::string dataPath;
private:
	std::vector<std::unique_ptr<InteractableObject2D>> objects;
};

