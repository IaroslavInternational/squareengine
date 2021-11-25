#include "ObjectsQueue.h"

ObjectsQueue::ObjectsQueue(MainPerson* hero, PersonContainer* persCon, InteractableObject2DContainer* Iobj)
{
	std::vector<size_t> idx;

	if (Iobj->elements.size() != 0)
	{
		for (size_t i = 0; i < Iobj->elements.size(); i++)
		{
			queue.push_back(Iobj->elements[i].get());
			idx.push_back(Iobj->elements[i].get()->layer);
		}
	}

	if (persCon->persons.size() != 0)
	{
		for (size_t i = 0; i < persCon->persons.size(); i++)
		{
			queue.push_back(persCon->persons[i].get());
			idx.push_back(persCon->persons[i].get()->layer);
		}
	}

	queue.push_back(hero);
	idx.push_back(hero->layer);

	if (Iobj->elements.size() != 0)
	{
		for (size_t i = 0; i < Iobj->elements.size(); i++)
		{
			queue.at(idx[i]) = Iobj->elements[i].get();
		}
	}

	if (persCon->persons.size() != 0)
	{
		for (size_t i = 0; i < persCon->persons.size(); i++)
		{
			if (Iobj->elements.size() != 0)
			{
				queue.at(idx[i + Iobj->elements.size()]) = persCon->persons[i].get();
			}
			else
			{
				queue.at(idx[i]) = persCon->persons[i].get();
			}
		}
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