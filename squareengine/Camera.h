#pragma once

#include "PhysicsEngine.h"
#include "PersonContainer.h"

class Camera
{
public:
	Camera(std::shared_ptr<Physics::PhysicsEngine> phEngPtr, PersonContainer* pcPtr);
public:
	void Translate(DirectX::XMFLOAT2 delta);
private:
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;
	PersonContainer*						pcPtr;
};
