#include "Scene.h"

Scene::Scene(std::string							 name,
			 std::shared_ptr<Window>				 wnd,	   std::string scData,
			 std::shared_ptr<Physics::PhysicsEngine> phEngine)
	:
	name(name),
	wnd(wnd),
	camera(std::make_shared<Camera>(phEngine, &pc)),
	gui(wnd, &pc, &hero, &layers, phEngine),
	sdr(scData),
	mdr(sdr.GetMainPersonDataPath()),
	pc( sdr.GetPersonContainerPath()),
	Iobj(sdr.GetInteractableObjectsDataPath()),
	hero(mdr, wnd, camera),
	layers(&hero, &pc, &Iobj),
	phEngine(phEngine)
{
	phEngine->LoadData(sdr.GetPhysicsDataPath());
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

	phEngine->CheckMainPersonCollision(&hero);
	hero.ProcessMoving(dt);
}

void Scene::Render(float dt)
{
	/* ��������� */

	wnd->Gfx().BeginFrame();	// ������ �����
	gui.Show();

	layers.Draw(wnd->Gfx());

	//test collision
	/*auto collisionState = pc.CheckCollision(hero.GetHitBox());

	if (collisionState.first)
	{
		collisionState.second->ActivateEffect();
	}*/
	
	phEngine->Draw(wnd->Gfx());
	wnd->Gfx().EndFrame();		// ����� �����

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