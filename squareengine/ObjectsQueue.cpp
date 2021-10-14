#include "ObjectsQueue.h"

ObjectsQueue::ObjectsQueue(MainPerson* hero, PersonContainer* persCon, InteractableObject2DContainer* Iobj)
{
	std::vector<size_t> idx;

	for (size_t i = 0; i < Iobj->objects.size(); i++)
	{
		queue.push_back(&Iobj->objects[i]);
		idx.push_back(Iobj->objects[i].layer);
	}

	for (size_t i = 0; i < persCon->persons.size(); i++)
	{
		queue.push_back(persCon->persons[i].get());
		idx.push_back(persCon->persons[i].get()->layer);
	}

	queue.push_back(hero);
	idx.push_back(hero->layer);

	for (size_t i = 0; i < Iobj->objects.size(); i++)
	{
		queue.at(idx[i]) = &Iobj->objects[i];
	}

	for (size_t i = Iobj->objects.size(); i < persCon->persons.size(); i++)
	{
		queue.at(idx[i]) = persCon->persons[i].get();
	}

	queue.at(idx.back()) = hero;
	
	queue.shrink_to_fit();
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
		
		queue[k]->LayerDown();
		queue[k - 1]->LayerUp();
	}
}

void ObjectsQueue::MoveUp(size_t k)
{
	if (k + 1 <= queue.size())
	{
		std::swap(queue[k], queue[k + 1]);

		queue[k]->LayerUp();
		queue[k + 1]->LayerDown();
	}
}