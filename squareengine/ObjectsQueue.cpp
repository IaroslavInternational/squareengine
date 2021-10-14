#include "ObjectsQueue.h"

ObjectsQueue::ObjectsQueue(MainPerson* hero, PersonContainer* persCon, InteractableObject2DContainer* Iobj)
{
	for (size_t i = 0; i < Iobj->objects.size(); i++)
	{
		queue.push_back(&Iobj->objects[i]);
	}

	for (size_t i = 0; i < persCon->persons.size(); i++)
	{
		queue.push_back(persCon->persons[i].get());
	}

	queue.push_back(hero);
}

void ObjectsQueue::Draw(Graphics& gfx)
{
	for (auto& obj : queue)
	{
		obj->Draw(gfx);
	}
}

void ObjectsQueue::DrawTransparent(Graphics& gfx, float deep)
{
	for (auto& obj : queue)
	{
	}
}

void ObjectsQueue::MoveDown(size_t k)
{
	if (k != 0)
	{
		std::swap(queue[k], queue[k - 1]);
	}
}

void ObjectsQueue::MoveUp(size_t k)
{
	if (k + 1 <= queue.size())
	{
		std::swap(queue[k], queue[k + 1]);
	}
}