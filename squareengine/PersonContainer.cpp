#include "PersonContainer.h"
#include "EngineFunctions.hpp"

PersonContainer::PersonContainer(std::string dataPath)
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

	if (j.size() != 0)
	{
		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			auto d = m.key();

			for (const auto& obj : j.at(d))
			{
				/* Получение имени объекта */

				std::string name = obj.at("name");
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

				/* Получение настроек хит-бокса */

				DirectX::XMFLOAT4 hb_coord;
				hb_coord.x = obj.at("hb-ltx");
				hb_coord.y = obj.at("hb-lty");
				hb_coord.z = obj.at("hb-rbx");
				hb_coord.w = obj.at("hb-rby");

				/********************************/

				/* Получение настроек эффекта */

				float eff_d = obj.at("eff-d");
				float eff_t = obj.at("eff-t");
				float eff_a = obj.at("eff-a");

				/******************************/

				/* Получение настройки скорости */

				float speed = obj.at("speed");

				/********************************/

				/* Получение настройки слоя */

				size_t layer = obj.at("layer");

				/****************************/

				/* Получение настройки анимаций */

				AnimationData aData;
				aData.pStart = obj.at("a-ps");
				aData.pEnd = obj.at("a-pe");
				aData.width = obj.at("a-fw");
				aData.height = obj.at("a-fh");
				aData.frames = obj.at("a-fa");
				aData.ft = obj.at("a-ft");

				/********************************/

				/* Получение пути скрипта */

				std::string scriptPath = obj.at("script");

				/**************************/

				/* Инициализация объекта */

				persons.push_back(std::make_unique<Person>(name, position, layer, pathToSprite, HitBox(name + std::string(" hitbox"), hb_coord), aData, scriptPath, speed, eff_d, eff_t, eff_a));

				/*************************/
			}
		}

		persons.shrink_to_fit();
	}
}

void PersonContainer::Process(float dt)
{
	for (auto& p : persons)
	{
		p->Process(dt);
	}
}

void PersonContainer::Draw(Graphics& gfx)
{
	if (persons.size() != 0)
	{
		for (auto& p : persons)
		{
			p->Draw(gfx);
		}
	}
}

void PersonContainer::Translate(DirectX::XMFLOAT2 delta)
{
	for (auto& p : persons)
	{
		p->Translate(delta);
	}
}

void PersonContainer::DeletePersonAt(size_t k)
{
	DeletePersonAt(persons.begin() + k);
}

void PersonContainer::DeletePersonAt(std::vector<std::unique_ptr<Person>>::iterator it)
{
	persons.erase(it);
}

std::pair<bool, Person*> PersonContainer::CheckCollision(HitBox& hb)
{
	for (auto& p : persons)
	{
		if (hb.IsCollide(p->GetHitBox()))
		{
			return { true, p.get() };
			break;
		}
	}

	return { false, nullptr };
}
