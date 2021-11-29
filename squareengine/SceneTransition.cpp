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

				/**********************************/
			
				break;
			}
		}
	}
}

void SceneTransition::Draw(Graphics& gfx)
{
	gfx.DrawFullscreenImage(*fade.get(), depth);
}

void SceneTransition::Appear(float dt)
{
	IsRun = true;

	if (depth <= 200.0f)
	{
		depth += 1 + dt * 50.0f;
	}
	else
	{
		depth = 200.0f;
		IsAppeared = true;
	}
}

void SceneTransition::Disappear(float dt)
{
	if (depth >= 1.0f)
	{
		depth -= 1 + dt * 50.0f;
	}
	else
	{
		depth = 1.0f;
		IsDisappeared = true;
		IsRun = false;
	}
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
