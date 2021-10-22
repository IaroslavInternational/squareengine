#include "App.h"

#include "imgui\imgui.h"
#include "EngineUtil.h"
#include "EngineFunctions.hpp"

namespace dx = DirectX;

App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(std::make_shared<Window>("SquareEngine 1.0")),
	phEngine(std::make_shared<Physics::PhysicsEngine>()),
	scriptCommander(TokenizeQuoted(commandLine))
{
	scenes.emplace("Scene 1", true);
	scenes.emplace("Scene 2", false);

	scene = std::make_unique<Scene>("Scene 1", wnd, "Scenes\\Scene 1\\scene_1.json", phEngine);
}

App::~App()
{}

int App::Go()
{
	while( true )
	{
		// ��������� ���� ���������
		if(const auto ecode = Window::ProcessMessages())
		{
			// ���� optional ����� ��������, �������
			return *ecode;
		}
		
		// ������ ����
		const auto dt = timer.Mark() * speed_factor;
		
		HandleInput(dt);
		DoFrame(dt);
	}
}

void App::HandleInput(float dt)
{
	for (auto& s : scenes)
	{
		if (s.second)
		{
			scene->ProcessInput(dt);
		}
	}
}

void App::DoFrame(float dt)
{
	for (auto& s : scenes)
	{
		if (s.second)
		{
			scene->Render(dt);

			// ��� �������� �����
			auto activeSceneName = scene->GetName();

			// ������ � ��������
			auto t = scene->IsOnTheSceneTrigger();

			if (t.second)
			{
				// ������ ����� ����� ��������
				for (auto it = scenes.begin(); it != scenes.end(); ++it)
				{
					if (it->first == t.first)
					{
						it->second = true;
						
						std::ostringstream oss;
						oss << "Scenes\\" << it->first << "\\scene_" << EngineFunctions::StrReplace(it->first, "Scene ", "") << ".json";

						scene = std::make_unique<Scene>(it->first, wnd, oss.str().c_str(), phEngine);
						break;
					}
				}

				// ������ ������ ����� ����������
				for (auto it = scenes.begin(); it != scenes.end(); ++it)
				{
					if (it->first == activeSceneName)
					{
						it->second = false;
						break;
					}
				}
			}
		}
	}
}