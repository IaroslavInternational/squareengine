#include "Scene.h"

Scene::Scene(std::string							 name,
			 std::shared_ptr<Window>				 wnd,	
			 std::string							 scData,
			 std::shared_ptr<Physics::PhysicsEngine> phEngine)
	:
	name(name),
	phEngine(phEngine),
	wnd(wnd),
	sdr(scData),
	camera(std::make_shared<Camera>(&hero, &persCon, &Iobj, phEngine, sdr.GetCameraDataPath())),
	mdr(sdr.GetMainPersonDataPath()),
	hero(mdr, wnd, camera),
	persCon(sdr.GetPersonContainerPath()),
	Iobj(sdr.GetInteractableObjectsDataPath()),
	objQueue(&hero, &persCon, &Iobj)
{
	phEngine->LoadData(sdr.GetPhysicsDataPath());
	camera->Init();
}

Scene::~Scene()
{}

/* ������ ����� */

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

	if (camera->GetNoClipState())
	{
		if (!wnd->CursorEnabled())
		{
			if (wnd->kbd.KeyIsPressed('W'))
			{
				camera->TranslateAll({ 0.0f, dt});
			}
			if (wnd->kbd.KeyIsPressed('A'))
			{
				camera->TranslateAll({ dt, 0.0f });
			}
			if (wnd->kbd.KeyIsPressed('S'))
			{
				camera->TranslateAll({ 0.0f, -dt});
			}
			if (wnd->kbd.KeyIsPressed('D'))
			{
				camera->TranslateAll({ -dt, 0.0f });
			}
		}
	}

	phEngine->CheckMainPersonCollision(&hero);
	Iobj.CheckCollision(&hero);

	if (!camera->GetNoClipState())
	{
		hero.ProcessMoving(dt);
	}
}

void Scene::Render(float dt)
{
	/* ��������� */

	objQueue.Draw(wnd->Gfx());	
	
	/*************/
}

std::pair<std::string, bool> Scene::IsOnTheSceneTrigger()
{
	// test
	if(hero.GetPosition().x >= 400.0f && hero.GetPosition().y >= 400.0f)
		return std::pair{ "Scene 2", true };

	return std::pair{ "NULL", false };
}

const std::string& Scene::GetName() const
{
	return name;
}

/****************/