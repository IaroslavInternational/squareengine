#pragma once

#include "MainPerson.h"
#include "PersonContainer.h"
#include "InteractableObject2DContainer.h"

class Layers
{
public:
	Layers(MainPerson* mp, PersonContainer* pc, InteractableObject2DContainer* obj);
public:
	void Draw(Graphics& gfx);
public:
	void MoveDown(size_t k);
	void MoveDown(std::vector<Object2D>::iterator it);
	void MoveUp(size_t k);
	void MoveUp(std::vector<Object2D>::iterator it);
private:
	std::vector<Object2D*> objects;
};

