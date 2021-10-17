#pragma once

#include "PhysicsEngine.h"
#include <memory>

class PersonContainer;
class InteractableObject2DContainer;

class Camera
{
public:
	Camera(PersonContainer* pc, InteractableObject2DContainer* Iobj, std::shared_ptr<Physics::PhysicsEngine> phEngPtr);
public:
	void Translate(DirectX::XMFLOAT2 delta);
private:
	PersonContainer* pc;
	InteractableObject2DContainer* Iobj;
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;
};
