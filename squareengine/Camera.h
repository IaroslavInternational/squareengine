#pragma once

#include "PhysicsEngine.h"
#include <memory>

class MainPerson;
class PersonContainer;
class InteractableObject2DContainer;
class TriggerContainer;

class Camera
{
public:
	friend class GUISystem;
public:
	Camera(MainPerson* hero, PersonContainer* pc, InteractableObject2DContainer* Iobj, TriggerContainer* trigCon, std::shared_ptr<Physics::PhysicsEngine> phEngPtr, std::string dataPath);
public:
	void Init();
	void Translate(DirectX::XMFLOAT2 delta);
	void TranslateAll(DirectX::XMFLOAT2 delta);
	bool GetNoClipState();
	void SetPosition(DirectX::XMFLOAT2 pos);
	void SetInitPosition(DirectX::XMFLOAT2 pos);
private:
	MainPerson*								hero;
	PersonContainer*						pc;
	InteractableObject2DContainer*			Iobj;
	TriggerContainer*						trigCon;
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;
private:
	std::string		  dataPath;
	bool			  noclip;
	float			  noclipSpeed;
	DirectX::XMFLOAT2 initPosition;
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
};
