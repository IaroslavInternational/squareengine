#include "App.h"

#include "imgui\imgui.h"
#include "EngineUtil.h"

namespace dx = DirectX;

App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(std::make_shared<Window>("Legacy")),
	scriptCommander(TokenizeQuoted(commandLine))
{
	scenes.emplace(std::make_unique<Scene>("Scene 1", wnd, "Scenes\\Scene 1\\scene_1.json"), true);
	scenes.emplace(std::make_unique<Scene>("Scene 2", wnd, "Scenes\\Scene 2\\scene_2.json"), false);
}

void App::HandleInput(float dt)
{
	for (auto& s : scenes)
	{
		if (s.second)
		{
			s.first->ProcessInput(dt);
		}
	}
}

void App::DoFrame(float dt)
{
	for (auto& s : scenes)
	{
		if (s.second)
		{
			s.first->Render(dt);

			// Имя активной сцены
			auto activeSceneName = s.first->GetName();

			// Данные о триггере
			auto t = s.first->IsOnTheSceneTrigger();

			if (t.second)
			{
				// Делаем новую сцену активной
				for (auto it = scenes.begin(); it != scenes.end(); ++it)
				{
					if (it->first->GetName() == t.first)
					{						
						it->second = true;
						it->first->ResetPos();						
						break;
					}
				}

				// Делаем старую сцену неактивной
				for (auto it = scenes.begin(); it != scenes.end(); ++it)
				{
					if (it->first->GetName() == activeSceneName)
					{						
						it->second = false;
						break;
					}
				}
			}
		}
	}
}

App::~App()
{}

int App::Go()
{
	while( true )
	{
		// Обработка всех сообщений
		if(const auto ecode = Window::ProcessMessages())
		{
			// Если optional имеет значение, выходим
			return *ecode;
		}
		
		// Логика игры
		const auto dt = timer.Mark() * speed_factor;
		
		HandleInput(dt);
		DoFrame(dt);
	}
}