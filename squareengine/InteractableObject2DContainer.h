#pragma once

#include "InteractableObject2D.h"
#include "ContainerBase.hpp"

class PhysicObject2D;

class InteractableObject2DContainer : public ContainerBase<std::unique_ptr<InteractableObject2D>>
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	InteractableObject2DContainer(std::string dataPath);
public:
	void Translate(DirectX::XMFLOAT2 delta);
	void CheckOverlap(PhysicObject2D* object);
};
