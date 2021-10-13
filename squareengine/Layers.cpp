#include "Layers.h"

Layers::Layers(MainPerson* mp, PersonContainer* pc, InteractableObject2DContainer* obj)
{
	for (size_t i = 0; i < obj->objects.size(); i++)
	{
		objects.push_back(&obj->objects[i]);
	}

	for (size_t i = 0; i < pc->persons.size(); i++)
	{
		objects.push_back(pc->persons[i].get());
	}

	objects.push_back(mp);
}

void Layers::Draw(Graphics& gfx)
{
	for (auto& obj : objects)
	{
		obj->Draw(gfx);
	}
}
