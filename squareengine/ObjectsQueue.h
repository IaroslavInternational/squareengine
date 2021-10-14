#pragma once

#include "MainPerson.h"
#include "PersonContainer.h"
#include "InteractableObject2DContainer.h"

class ObjectsQueue
{
public:
	friend class GUISystem;
public:
	ObjectsQueue(MainPerson* hero, PersonContainer* persCon, InteractableObject2DContainer* Iobj);
public:
	void Draw(Graphics& gfx);
	void DrawTransparent(Graphics& gfx, float deep);
public:
	void MoveDown(size_t k);
	void MoveUp(size_t k);
private:
	std::vector<Object2D*> queue;
};

