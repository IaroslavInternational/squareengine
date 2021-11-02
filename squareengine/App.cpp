#include "App.h"

#include "imgui\imgui.h"
#include "EngineUtil.h"
#include "EngineFunctions.hpp"

App::App(const std::string& commandLine, const std::string& projectName)
	:
	wnd(std::make_shared<Window>("SquareEngine 1.0")),
	phEngine(std::make_shared<Physics::PhysicsEngine>()),
	commandLine(commandLine),
	projectName(projectName),
	scriptCommander(TokenizeQuoted(commandLine))
{
	{
		std::ostringstream dataPath;
		dataPath << "Projects\\" << projectName << "\\project_settings.json";

		std::ifstream dataFile(dataPath.str());
		if (!dataFile.is_open())
		{
			throw ("Не удаётся открыть файл с данными о проекте");
		}

		json j;
		dataFile >> j;

		dataFile.close();

		size_t counter = 1;
		std::ostringstream s;

		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			auto& d = m.key();

			if (d.find("scenes") != d.npos)
			{
				for (const auto& obj : j.at(d))
				{
					s << "scene " << counter;

					while (obj.contains(s.str()))
					{
						s.str("");

						s << "scene " << counter;
						/* Получение имени объекта */

						bool sceneState = obj.at(s.str().c_str());

						/***************************/

						s.str("");

						/* Инициализация */

						s << "Scene " << counter;

						scenes.emplace(s.str(), sceneState);

						/*****************/
						
						s.str("");
						
						counter++;
						s << "scene " << counter;
					}

					s.str("");
				}
			}
		}
	}

	std::ostringstream dir;
	dir << "Projects\\" << projectName << "\\Scenes\\Scene 1\\scene_1.json";

	scene = std::make_unique<Scene>("Scene 1", wnd, dir.str(), phEngine);
	gui = std::make_shared<GUISystem>(scene.get());
}

int App::Go()
{
	// Игровой цикл
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

void App::HandleInput(float dt)
{
	scene->ProcessInput(dt);
}

void App::DoFrame(float dt)
{
	wnd->Gfx().BeginFrame();	// Начало кадра

	wnd->Gfx().DrawBackground();	// Отрисовка заднего фона
	wnd->Gfx().DrawGrid();			// Отрисовка сетки

	for (auto& s : scenes)
	{
		if (s.second)
		{			
			// Имя активной сцены
			std::string activeSceneName = scene->GetName();

			// Если переход на другую сцену вызван из интерфейса
			if (gui->UpdatingScene().first)
			{
				std::ostringstream oss;
				oss << "Projects\\" << projectName << "\\Scenes\\" << gui->UpdatingScene().second << "\\scene_"
					<< EngineFunctions::StrReplace(gui->UpdatingScene().second, "Scene ", "") << ".json";
				
				// Делаем новую сцену активной
				for (auto it = scenes.begin(); it != scenes.end(); ++it)
				{
					if (it->first == gui->UpdatingScene().second)
					{
						it->second = true;
					}
				}

				// Делаем старую сцену неактивной
				for (auto it = scenes.begin(); it != scenes.end(); ++it)
				{
					if (it->first == activeSceneName)
					{
						it->second = false;
						break;
					}
				}

				scene = std::make_unique<Scene>(gui->UpdatingScene().second, wnd, oss.str(), phEngine);
				gui->LoadScene(scene.get());
			}

			scene->Render(dt);	// Отрисовка содержания сцены

			// Данные о триггере
			auto t = scene->IsOnTheSceneTrigger();
			
			if (t.second)
			{
				// Делаем новую сцену активной
				for (auto it = scenes.begin(); it != scenes.end(); ++it)
				{
					if (it->first == t.first)
					{
						it->second = true;
						
						std::ostringstream oss;
						oss << "Projects\\"<< projectName << "\\Scenes\\" << it->first << "\\scene_" 
							<< EngineFunctions::StrReplace(it->first, "Scene ", "") << ".json";

						scene = std::make_unique<Scene>(it->first, wnd, oss.str(), phEngine);						
						gui->LoadScene(scene.get());
						
						break;
					}
				}

				// Делаем старую сцену неактивной
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

	phEngine->Draw(wnd->Gfx());	// Отрисовка объектов физического движка

	gui->Show(dt);	// Отрисовка интерфейса

	wnd->Gfx().EndFrame();	// Конец кадра
}