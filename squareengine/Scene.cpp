#include "Scene.h"

Scene::Scene(std::string name,	std::shared_ptr<Window> wnd, 
			 std::string data)
	:
	name(name),
	wnd(wnd),
	gui(wnd, &pc, &hero),
	sdr(data),
	mdr(sdr.GetMainPersonDataPath()),
	pc( sdr.GetPersonContainerPath()),
	hero(mdr, wnd)
{}

Scene::~Scene()
{}

/* Методы сцены */

void Scene::ProcessInput(float dt)
{
	while (const auto e = wnd->kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd->CursorEnabled())
			{
				wnd->DisableCursor();
				wnd->mouse.EnableRaw();
			}
			else
			{
				wnd->EnableCursor();
				wnd->mouse.DisableRaw();
			}
			break;
		default:
			break;
		}
	}

	hero.ProcessMoving(dt);
}

void Scene::Render(float dt)
{
	/* Отрисовка */

	wnd->Gfx().BeginFrame();	// Начало кадра

	gui.Show();
	
	pc.Draw(wnd->Gfx());
	hero.Draw();

	wnd->Gfx().EndFrame();		// Конец кадра

	/*************/
}

std::pair<std::string, bool> Scene::IsOnTheSceneTrigger()
{
	return std::pair{ "NULL", false };
}

const std::string& Scene::GetName() const
{
	return name;
}

/****************/