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
	triggers(sdr.GetTriggerContainerDataPath()),
	camera(std::make_shared<Camera>(&hero, &persons, &Iobjects, &triggers, phEngine, sdr.GetCameraDataPath())),
	mdr(sdr.GetMainPersonDataPath()),
	hero(mdr, wnd, camera),
	persons(sdr.GetPersonContainerPath()),
	Iobjects(sdr.GetInteractableObjectsDataPath()),
	objQueue(&hero, &persons, &Iobjects)
{
	phEngine->LoadData(sdr.GetPhysicsDataPath());
	camera->Init();

	sdr.~SceneDataReader();
	mdr.~MainPersonDataReader();
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

	phEngine->CheckCollision(&hero);

	for (size_t i = 0; i < persons.GetSize(); i++)
	{
		phEngine->CheckCollision(persons.Get(i).get());
	}
	
	Iobjects.CheckOverlap(&hero);

	if (!camera->GetNoClipState())
	{
		hero.Process(dt);
	}

	persons.Process(dt);
}

void Scene::Render(float dt)
{
	/* Отрисовка */

	objQueue.Draw(wnd->Gfx());	
	triggers.Draw(wnd->Gfx());

	/*************/
}

std::optional<std::string> Scene::IsOnTheSceneTrigger()
{
	auto trigger = triggers.Check(hero.GetHitBox());

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