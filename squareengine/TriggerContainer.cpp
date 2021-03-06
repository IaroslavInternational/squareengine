#include "TriggerContainer.h"
#include "EngineFunctions.hpp"

TriggerContainer::TriggerContainer(std::string dataPath)
	:
	ContainerBase(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("?? ??????? ??????? ???? ? ??????? ? ??????????");
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
				/* ????????? ????? ???????? */

				std::string name = d;
				std::transform(name.begin(), name.end(), name.begin(), tolower);

				/***************************/

				/* ????????? ??????? */

				float start_x = obj.at("start-x");
				float start_y = obj.at("start-y");
				float end_x   = obj.at("end-x");
				float end_y   = obj.at("end-y");

				/*********************/

				/* ????????? ???? ???????? */

				size_t t = obj.at("type");
				TriggerType type = static_cast<TriggerType>(t);

				/***************************/

				/* ????????? ????? ???????? */
				
				std::string goal = obj.at("goal");
				
				/****************************/

				/* ????????????? ??????? */

				Add(Trigger(name, start_x, start_y, end_x, end_y, type, goal));

				/*************************/
			}
		}

		elements.shrink_to_fit();
	}
}

void TriggerContainer::Draw(Graphics& gfx)
{
	for (auto& t : elements)
	{
		if (t.GetLine().IsVisible())
		{
			t.Draw(gfx);
		}
	}
}

void TriggerContainer::Translate(const DirectX::XMFLOAT2& delta)
{
	for (auto& t : elements)
	{
		t.Translate(delta);
	}
}

void TriggerContainer::UpdateLineAt(size_t k, Physics::Line line)
{
	elements.at(k).SetLine(line);
}

std::optional<Trigger> TriggerContainer::Check(HitBox hitbox)
{
	for (auto& t : elements)
	{
		if (t.IsCollide(hitbox))
		{
			return t;
		}
	}

	return std::nullopt;
}