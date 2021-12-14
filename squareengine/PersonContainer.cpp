#include "PersonContainer.h"

#include "EngineFunctions.hpp"

PersonContainer::PersonContainer(std::string dataPath)
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
				size_t key_r = obj.at("chr-r");
				size_t key_g = obj.at("chr-g");
				size_t key_b = obj.at("chr-b");

				Color key(key_r, key_g, key_b);

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

				/* Получение настройки атаки */

				float health = obj.at("hlt");
				float damage = obj.at("dmg");

				/*****************************/

				/* Получение настройки физики */

				float j_h	  = obj.at("j-h");
				float gravity = obj.at("gravity");

				/*****************************/

				/* Инициализация объекта */

				Add(std::make_unique<Person>(name, position, health, damage, layer, pathToSprite, key, HitBox(name + std::string(" hitbox"), hb_coord), aData, scriptPath, j_h, gravity, speed, eff_d, eff_t, eff_a));

				/*************************/
			}
		}

		elements.shrink_to_fit();
	}
}

void PersonContainer::Process(float dt)
{
	if (IsScriptsRunning)
	{
		for (auto& p : elements)
		{
			p->Process(dt);
		}
	}
}

void PersonContainer::Draw(Graphics& gfx)
{
	if (elements.size() != 0)
	{
		for (auto& p : elements)
		{
			p->Draw(gfx);
		}
	}
}

void PersonContainer::Translate(DirectX::XMFLOAT2 delta)
{
	for (auto& p : elements)
	{
		p->Translate(delta);
	}
}

std::unique_ptr<Person>& PersonContainer::Get(size_t i)
{
	return elements.at(i);
}