#include "InteractableObject2DContainer.h"
#include "PhysicObject2D.h"

#include "EngineFunctions.hpp"

InteractableObject2DContainer::InteractableObject2DContainer(std::string dataPath)
	:
	ContainerBase(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными об о объектах");
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
				/* Получение имени объекта */

				std::string name = m.key();
				std::transform(name.begin(), name.end(), name.begin(), tolower);

				/***************************/

				/* Получение позиции */

				float pos_x = obj.at("pos-x");
				float pos_y = obj.at("pos-y");

				DirectX::XMFLOAT2 position = { pos_x, pos_y };

				/*********************/

				/* Получение пути к изображению */

				std::string pathToSprite = obj.at("path");
				size_t key_r = obj.at("chr-r");
				size_t key_g = obj.at("chr-g");
				size_t key_b = obj.at("chr-b");

				Color key(key_r, key_g, key_b);

				/********************************/

				/* Получение настройки слоя */

				size_t layer = obj.at("layer");

				/****************************/

				/* Получение настроек хит-бокса */

				DirectX::XMFLOAT4 hb_coord;
				hb_coord.x = obj.at("hb-ltx");
				hb_coord.y = obj.at("hb-lty");
				hb_coord.z = obj.at("hb-rbx");
				hb_coord.w = obj.at("hb-rby");

				/********************************/

				/* Получение настроек коллизий */

				float gDeep = obj.at("g-deep");
				float gAble = obj.at("g-able");

				/*******************************/

				/* Инициализация объекта */

				Add(std::make_unique<InteractableObject2D>(name, position, layer, pathToSprite, key, HitBox(name + std::string(" hitbox"), hb_coord), gDeep, gAble));

				/*************************/
			}
		}

		elements.shrink_to_fit();
	}
}

void InteractableObject2DContainer::Translate(DirectX::XMFLOAT2 delta)
{
	if (elements.size() != 0)
	{
		for (auto& obj : elements)
		{
			obj->Translate(delta);
		}
	}
}

void InteractableObject2DContainer::CheckOverlap(PhysicObject2D* object)
{
	for (auto& obj : elements)
	{
		if (object->GetLayer() < obj->GetLayer() && obj->IsGhostable())
		{
			obj->SetGhostState(object->GetHitBox().IsCollide(obj->GetHitBox()));
		}
	}
}
