#pragma once

#include "Camera.h"

#include "ObjectsQueue.h"
#include "PhysicsEngine.h"
#include "TriggerContainer.h"

class Map
{
public:
	friend class Scene;
	friend class GUISystem;
public:
	Map(std::string heroData, std::string personData, std::string IobjectsData, std::string triggerData, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera);
public:
	void Translate(DirectX::XMFLOAT2 delta);
private:
	MainPersonDataReader mdr;
private:
	/* Объекты */

	MainPerson					  hero;
	PersonContainer				  persons;
	InteractableObject2DContainer Iobjects;
	ObjectsQueue				  objQueue;
	TriggerContainer			  triggers;

	/***********/
};
