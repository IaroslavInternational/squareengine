#include "InteractableObject2DContainer.h"
#include "MainPerson.h"

#include "EngineFunctions.hpp"

InteractableObject2DContainer::InteractableObject2DContainer(std::string dataPath)
	:
	dataPath(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� �� � ��������");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	if (j.size() != 0)
	{
		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			auto d = m.key();

			for (const auto& obj : j.at(d))
			{
				/* ��������� ����� ������� */

				std::string name = m.key();
				std::transform(name.begin(), name.end(), name.begin(), tolower);

				/***************************/

				/* ��������� ������� */

				float pos_x = obj.at("pos-x");
				float pos_y = obj.at("pos-y");

				DirectX::XMFLOAT2 position = { pos_x, pos_y };

				/*********************/

				/* ��������� ���� � ����������� */

				std::string pathToSprite = obj.at("path");

				/********************************/

				/* ��������� ��������� ���� */

				size_t layer = obj.at("layer");

				/****************************/

				/* ��������� �������� ���-����� */

				DirectX::XMFLOAT4 hb_coord;
				hb_coord.x = obj.at("hb-ltx");
				hb_coord.y = obj.at("hb-lty");
				hb_coord.z = obj.at("hb-rbx");
				hb_coord.w = obj.at("hb-rby");

				/********************************/

				/* ��������� �������� �������� */

				float gDeep = obj.at("g-deep");
				float gAble = obj.at("g-able");

				/*******************************/

				/* ������������� ������� */

				objects.emplace_back(std::make_unique<InteractableObject2D>(name, position, layer, pathToSprite, HitBox(name + std::string(" hitbox"), hb_coord), gDeep, gAble));

				/*************************/
			}
		}

		objects.shrink_to_fit();
	}
}

void InteractableObject2DContainer::Translate(DirectX::XMFLOAT2 delta)
{
	if (objects.size() != 0)
	{
		for (auto& obj : objects)
		{
			obj->Translate(delta);
		}
	}
}

void InteractableObject2DContainer::DeleteObjectAt(size_t k)
{
	DeleteObjectAt(objects.begin() + k);
}

void InteractableObject2DContainer::DeleteObjectAt(std::vector<std::unique_ptr<InteractableObject2D>>::iterator it)
{
	objects.erase(it);
}

void InteractableObject2DContainer::CheckOverlap(MainPerson* hero)
{
	for (auto& obj : objects)
	{
		if (hero->GetLayer() < obj->GetLayer() && obj->IsGhostable())
		{
			obj->SetGhostState(hero->GetHitBox().IsCollide(obj->GetHitBox()));
		}
	}
}
