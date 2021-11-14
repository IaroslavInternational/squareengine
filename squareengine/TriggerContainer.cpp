#include "TriggerContainer.h"
#include "EngineFunctions.hpp"

TriggerContainer::TriggerContainer(std::string dataPath)
	:
	dataPath(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о триеггерах");
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
				/* Получение имени триггера */

				std::string name = d;
				std::transform(name.begin(), name.end(), name.begin(), tolower);

				/***************************/

				/* Получение позиции */

				float pos_ltx = obj.at("pos-ltx");
				float pos_lty = obj.at("pos-lty");
				float pos_rbx = obj.at("pos-rbx");
				float pos_rby = obj.at("pos-rby");

				/*********************/

				/* Получение типа триггера */

				size_t t = obj.at("type");
				TriggerType type = static_cast<TriggerType>(t);

				/***************************/

				/* Получение целей триггера */
				
				std::string goal = obj.at("goal");
				
				/****************************/

				/* Инициализация объекта */

				triggers.push_back(Trigger(name, pos_ltx, pos_lty, pos_rbx, pos_rby, type, goal));

				/*************************/
			}
		}

		triggers.shrink_to_fit();
	}
}

void TriggerContainer::Draw(Graphics& gfx)
{
	for (auto& t : triggers)
	{
		t.Draw(gfx);
	}
}

std::optional<std::pair<TriggerType, std::string>> TriggerContainer::Check(HitBox hitbox)
{
	for (auto& t : triggers)
	{
		if (t.IsCollide(hitbox))
		{
			return std::pair<TriggerType, std::string>(t.GetType(), t.GetName());
		}
	}

	return std::nullopt;
}

const Trigger& TriggerContainer::GetTriggerByName(std::string name)
{
	for (auto& t : triggers)
	{
		if (t.GetName() == name)
		{
			return t;
		}
	}
}
