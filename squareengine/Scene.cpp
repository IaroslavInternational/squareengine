#include "Scene.h"
#include "EngineUtil.h"

#include <sstream>

Scene::Scene(std::string name,	std::shared_ptr<Window> _wnd, 
			 std::string data)
	:
	wnd(_wnd),
	name(name),
	sdr(data),
	pc(sdr.GetPersonContainerPath()),
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

	DirectX::XMFLOAT2 dir = { 0.0f,0.0f };
	if (!wnd->CursorEnabled())
	{
		if (wnd->kbd.KeyIsPressed('W'))
		{
			dir.y -= 1.0f;
		}
		if (wnd->kbd.KeyIsPressed('A'))
		{
			dir.x -= 1.0f;
		}
		if (wnd->kbd.KeyIsPressed('S'))
		{
			dir.y += 1.0f;
		}
		if (wnd->kbd.KeyIsPressed('D'))
		{
			dir.x += 1.0f;
		}
	}
}

void Scene::Render(float dt)
{
	/* Начало кадра */

	wnd->Gfx().BeginFrame();

	gui.Show();
	pc.Draw(wnd->Gfx());
	
	if (pc.TestCollision())
	{
		gui.AddLog("Collision\n");
	}		
	
	pc.Process();

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