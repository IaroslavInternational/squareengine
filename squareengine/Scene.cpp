#include "Scene.h"

Scene::Scene(std::string							 name,
			 std::shared_ptr<Window>				 wnd,	
			 std::string							 scData,
			 std::shared_ptr<Physics::PhysicsEngine> phEngine)
	:
	name(name),
	phEngine(phEngine),
	wnd(wnd),
	camera(std::make_shared<Camera>(&persCon, &Iobj, phEngine)),
	gui(wnd, &hero, &persCon, &Iobj, &objQueue, phEngine),
	sdr(scData),
	mdr(sdr.GetMainPersonDataPath()),
	hero(mdr, wnd, camera),
	persCon(sdr.GetPersonContainerPath()),
	Iobj(sdr.GetInteractableObjectsDataPath()),
	objQueue(&hero, &persCon, &Iobj)
{
	phEngine->LoadData(sdr.GetPhysicsDataPath());
}

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

	phEngine->CheckMainPersonCollision(&hero);
	Iobj.CheckCollision(&hero); // test
	hero.ProcessMoving(dt);
}

void Scene::Render(float dt)
{
	/* Отрисовка */

	wnd->Gfx().BeginFrame();	// Начало кадра
	
	gui.Show();

	objQueue.Draw(wnd->Gfx());
	
	phEngine->Draw(wnd->Gfx());
	
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