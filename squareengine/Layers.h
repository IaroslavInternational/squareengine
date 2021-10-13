#pragma once

#include "MainPerson.h"
#include "PersonContainer.h"
#include "InteractableObject2DContainer.h"

class Layers
{
public:
	friend class GUISystem;
public:
	Layers(MainPerson* mp, PersonContainer* pc, InteractableObject2DContainer* obj);
public:
	void Draw(Graphics& gfx);
public:
	void MoveDown(size_t k);
	void MoveUp(size_t k);
private:
	std::vector<Object2D*> objects;
};

