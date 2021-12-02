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
	camera(std::make_shared<Camera>(&world, &world.hero, phEngine, sdr.GetCameraDataPath()))
{
	phEngine->LoadData(sdr.GetPhysicsDataPath());
	camera->Init();

	sdr.~SceneDataReader();
}

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

	phEngine->CheckCollision(&world.hero);

	for (size_t i = 0; i < world.persons.GetSize(); i++)
	{
		phEngine->CheckCollision(world.persons.Get(i).get());
	}
	
	world.Iobjects.CheckOverlap(&world.hero);

	if (!camera->GetNoClipState())
	{
		world.hero.Process(dt);
	}

	world.persons.Process(dt);
}

void Scene::Render(float dt)
{
	/* Отрисовка */

	world.Draw(wnd->Gfx());
	world.objQueue.Draw(wnd->Gfx());	
	world.triggers.Draw(wnd->Gfx());

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