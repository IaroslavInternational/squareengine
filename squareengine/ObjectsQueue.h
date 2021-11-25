#pragma once

#include "MainPerson.h"
#include "PersonContainer.h"
#include "InteractableObject2DContainer.h"

class ObjectsQueue
{
public:
	friend class GUISystem;
public:
	ObjectsQueue(MainPerson* hero, PersonContainer* persons, InteractableObject2DContainer* Iobjects);
public:
	void Draw(Graphics& gfx);
public:
	void MoveDown(size_t k);
	void MoveUp(size_t k);
	void DeleteObjectAt(std::string name);
private:
	std::vector<Object2D*> queue;
};

