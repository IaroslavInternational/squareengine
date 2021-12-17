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
	world("Assets/Images/map.bmp", sdr.GetMainPersonDataPath(), sdr.GetPersonContainerPath(), sdr.GetInteractableObjectsDataPath(), sdr.GetTriggerContainerDataPath(), wnd, camera),
	camera(std::make_shared<Camera>(&world, &world.hero, phEngine, sdr.GetCameraDataPath())),
	fs(wnd)
{
	phEngine->LoadData(sdr.GetPhysicsDataPath());
	camera->Init();

	fs.SetFighter(&world.hero);
	
	for (size_t i = 0; i < world.persons.GetSize(); i++)
	{
		fs.Add(world.persons.Get(i).get());
	}

	sdr.~SceneDataReader();
}

/* ћетоды сцены */

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
			camera->ToggleNoClip();
			break;
		default:
			break;
		}
	}

	phEngine->CheckCollision(&world.hero);

	for (size_t i = 0; i < world.persons.GetSize(); i++)
	{
		phEngine->CheckCollision(world.persons.Get(i).get());
	}

	if (wnd->CursorEnabled())
	{
		if (wnd->kbd.KeyIsPressed('W'))
		{
			camera->TranslateAll({ 0.0f, dt });
		}
		if (wnd->kbd.KeyIsPressed('A'))
		{
			camera->TranslateAll({ dt, 0.0f });
		}
		if (wnd->kbd.KeyIsPressed('S'))
		{
			camera->TranslateAll({ 0.0f, -dt });
		}
		if (wnd->kbd.KeyIsPressed('D'))
		{
			camera->TranslateAll({ -dt, 0.0f });
		}
	}
	
	world.Iobjects.CheckOverlap(&world.hero);

	if (!wnd->CursorEnabled())
	{
		world.hero.Process(dt);
	}

	world.persons.Process(dt);

	// ¬тора€ проверка физики лишн€€ ?
	/*phEngine->CheckCollision(&world.hero);

	for (size_t i = 0; i < world.persons.GetSize(); i++)
	{
		phEngine->CheckCollision(world.persons.Get(i).get());
	}*/

	fs.ProcessFight();
}

void Scene::Render(float dt)
{
	/* ќтрисовка */

	world.Draw(wnd->Gfx());
	world.objQueue.Draw(wnd->Gfx());	
	world.triggers.Draw(wnd->Gfx());
	fs.DrawDebug();

	/*************/
}

std::optional<std::string> Scene::IsOnTheSceneTrigger()
{
	auto trigger = world.triggers.Check(world.hero.GetHitBox());

	if (trigger.has_value())
	{
		if (trigger.value().GetType() == TriggerType::SceneTrigger)
		{
			return trigger.value().GetGoal();
		}
	}

	return std::nullopt;
}

const std::string& Scene::GetName() const
{
	return name;
}

/****************/