#include "SceneTransition.h"

#include "Graphics.h"
#include "EngineFunctions.hpp"

SceneTransition::SceneTransition(std::string dataPath)
	:
	dataPath(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о переходах");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	if (j.size() != 0)
	{
		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			auto& d = m.key();

			for (const auto& obj : j.at(d))
			{
				/* Получение путей к изображениям */

				fade = std::make_unique<Surface2D>(obj.at("fade"));
				dawn = std::make_unique<Surface2D>(obj.at("dawn"));

				depth.speed = obj.at("d-s");
				depth.min   = obj.at("d-min");
				depth.max   = obj.at("d-max");
				depth.k	    = depth.min;

				/**********************************/
			
				break;
			}
		}
	}
}

void SceneTransition::Draw(Graphics& gfx)
{
	gfx.DrawFullscreenImage(*fade.get(), depth.k);
}

void SceneTransition::Appear(float dt)
{
	IsRun = true;

	if (depth.k <= depth.max)
	{
		depth.k += 1 + dt * depth.speed;
	}
	else
	{
		depth.k = depth.max;
		IsAppeared = true;
	}
}

void SceneTransition::Disappear(float dt)
{
	if (depth.k >= depth.min)
	{
		depth.k -= 1 + dt * depth.speed;
	}
	else
	{
		depth.k = depth.min;
		IsDisappeared = true;
		IsRun = false;
	}
}

void SceneTransition::Refresh()
{
	IsAppeared = false;
	IsDisappeared = false;
	IsRun = false;
}

bool SceneTransition::Appeared()
{
	return IsAppeared;
}

bool SceneTransition::Disappeared()
{
	return IsDisappeared;
}

bool SceneTransition::IsRunning()
{
	return IsRun;
}
