#include "ObjectsQueue.h"

ObjectsQueue::ObjectsQueue(MainPerson* hero, PersonContainer* persCon, InteractableObject2DContainer* Iobj)
{
	std::vector<size_t> idx;

	for (size_t i = 0; i < Iobj->objects.size(); i++)
	{
		queue.push_back(Iobj->objects[i].get());
		idx.push_back(Iobj->objects[i].get()->layer);
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
		queue.at(idx[i]) = Iobj->objects[i].get();
	}

	for (size_t i = 0; i < persCon->persons.size(); i++)
	{
		queue.at(idx[i + Iobj->objects.size()]) = persCon->persons[i].get();
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

void ObjectsQueue::MoveDown(size_t k)
{
	if (k != 0)
	{
		std::swap(queue[k], queue[k - 1]);
		
		queue[k]->LayerUp();
		queue[k - 1]->LayerDown();
	}
}

void ObjectsQueue::MoveUp(size_t k)
{
	if (k + 1 <= queue.size())
	{
		std::swap(queue[k], queue[k + 1]);

		queue[k]->LayerDown();
		queue[k + 1]->LayerUp();
	}
}

void ObjectsQueue::DeleteObjectAt(std::string name)
{
	size_t k;
	for (k = 0; k < queue.size(); k++)
	{
		if (queue[k]->GetName() == name)
		{
			queue.erase(queue.begin() + k);
			
			break;
		}
	}

	for (size_t i = k; i < queue.size(); i++)
	{
		queue[i]->layer -= 1;
	}
}