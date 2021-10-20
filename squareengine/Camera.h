#pragma once

#include "PhysicsEngine.h"
#include <memory>

class MainPerson;
class PersonContainer;
class InteractableObject2DContainer;

class Camera
{
public:
	friend class GUISystem;
public:
	Camera(MainPerson* hero, PersonContainer* pc, InteractableObject2DContainer* Iobj, std::shared_ptr<Physics::PhysicsEngine> phEngPtr);
public:
	void Init();
	void Translate(DirectX::XMFLOAT2 delta);
	void TranslateAll(DirectX::XMFLOAT2 delta);
	bool GetNoClipState();
	void SetPosition(DirectX::XMFLOAT2 pos);
	void SetInitPosition(DirectX::XMFLOAT2 pos);
private:
	MainPerson* hero;
	PersonContainer* pc;
	InteractableObject2DContainer* Iobj;
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;
private:
	bool noclip = true;
	float noclipSpeed = 400.0f;
	DirectX::XMFLOAT2 initPosition = {0.0f, 0.0f};
	DirectX::XMFLOAT2 position = {0.0f, 0.0f};
};
