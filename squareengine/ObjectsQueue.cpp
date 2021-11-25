#include "ObjectsQueue.h"

ObjectsQueue::ObjectsQueue(MainPerson* hero, PersonContainer* persons, InteractableObject2DContainer* Iobjects)
{
	std::vector<size_t> idx;

	if (Iobjects->elements.size() != 0)
	{
		for (size_t i = 0; i < Iobjects->elements.size(); i++)
		{
			queue.push_back(Iobjects->elements[i].get());
			idx.push_back(Iobjects->elements[i].get()->layer);
		}
	}

	if (persons->elements.size() != 0)
	{
		for (size_t i = 0; i < persons->elements.size(); i++)
		{
			queue.push_back(persons->elements[i].get());
			idx.push_back(persons->elements[i].get()->layer);
		}
	}

	queue.push_back(hero);
	idx.push_back(hero->layer);

	if (Iobjects->elements.size() != 0)
	{
		for (size_t i = 0; i < Iobjects->elements.size(); i++)
		{
			queue.at(idx[i]) = Iobjects->elements[i].get();
		}
	}

	if (persons->elements.size() != 0)
	{
		for (size_t i = 0; i < persons->elements.size(); i++)
		{
			if (Iobjects->elements.size() != 0)
			{
				queue.at(idx[i + Iobjects->elements.size()]) = persons->elements[i].get();
			}
			else
			{
				queue.at(idx[i]) = persons->elements[i].get();
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