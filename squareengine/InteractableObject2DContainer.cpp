#include "InteractableObject2DContainer.h"

#include "EngineFunctions.hpp"

InteractableObject2DContainer::InteractableObject2DContainer(std::string dataPath)
	:
	dataPath(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными об о объектах");
	}

	json j;
	dataFile >> j;

	dataFile.close();

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

			/* Инициализация объекта */

			objects.emplace_back(std::make_unique<InteractableObject2D>(name, position, layer, pathToSprite, HitBox(name + std::string(" hitbox"), hb_coord)));

			/*************************/
		}
	}

	objects.shrink_to_fit();
}

void InteractableObject2DContainer::Translate(DirectX::XMFLOAT2 delta)
{
	for (auto& obj : objects)
	{
		obj->Translate(delta);
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

void InteractableObject2DContainer::CheckCollision(HitBox hb)
{
	for (auto& obj : objects)
	{
		obj->SetGhostState(hb.IsCollide(obj->GetHitBox()));
	}
}
