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

				float start_x = obj.at("start-x");
				float start_y = obj.at("start-y");
				float end_x   = obj.at("end-x");
				float end_y   = obj.at("end-y");

				/*********************/

				/* Получение типа триггера */

				size_t t = obj.at("type");
				TriggerType type = static_cast<TriggerType>(t);

				/***************************/

				/* Получение целей триггера */
				
				std::string goal = obj.at("goal");
				
				/****************************/

				/* Инициализация объекта */

				triggers.push_back(Trigger(name, start_x, start_y, end_x, end_y, type, goal));

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
		if (t.GetLine().IsVisible())
		{
			t.Draw(gfx);
		}
	}
}

void TriggerContainer::Translate(const DirectX::XMFLOAT2& delta)
{
	for (auto& t : triggers)
	{
		t.Translate(delta);
	}
}

void TriggerContainer::UpdateLineAt(size_t k, Physics::Line line)
{
	triggers.at(k).SetLine(line);
}

std::optional<Trigger> TriggerContainer::Check(HitBox hitbox)
{
	for (auto& t : triggers)
	{
		if (t.IsCollide(hitbox))
		{
			return t;
		}
	}

	return std::nullopt;
}

void TriggerContainer::AddTrigger(Trigger trigger)
{
	triggers.push_back(trigger);
}

void TriggerContainer::DeleteAt(std::vector<Trigger>::iterator it)
{
	triggers.erase(it);
}

size_t TriggerContainer::GetTriggersAmount()
{
	return triggers.size();
}
