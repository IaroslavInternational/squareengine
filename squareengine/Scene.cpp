#include "Scene.h"
#include "EngineUtil.h"

#include <sstream>

Scene::Scene(std::string name,	std::shared_ptr<Window> _wnd, 
			 std::string data)
	:
	wnd(_wnd),
	name(name),
	sdr(data),
	mdr(sdr.GetMainPersonDataPath()),
	pc(sdr.GetPersonContainerPath()),
	hero(mdr, wnd),
	gui(wnd, &pc)
{}

Scene::~Scene()
{}

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
		case VK_SPACE:
			//person.ActivateEffect();
		default:
			break;
		}
	}
}

void Scene::Render(float dt)
{
	/* Начало кадра */

	wnd->Gfx().BeginFrame();

	gui.Show();
	
	pc.Draw(wnd->Gfx());
	
	hero.ProcessMoving(dt);
	hero.Draw();


	wnd->Gfx().EndFrame();

	/***************/
}

std::pair<std::string, bool> Scene::IsOnTheSceneTrigger()
{
	return std::pair{ "NULL", false };
}

std::string Scene::GetName() const
{
	return name;
}