#include "MainPerson.h"

MainPerson::MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera)
	:
	AliveObject2D(data.name, data.position, data.health, data.damage, HitBox(data.name + std::string(" hitbox"), data.hb_coord), data.layer, data.pathToSprite, data.key, data.speed, 8, 300),
	dataPath(data.dataPath),
	holdTime(data.anim_ft),
	wnd(wnd),
	camera(camera),
	cameraMode(static_cast<CameraMode>(data.camMode))
{
	effect.Duration = data.eff_d;
	effect.Time = data.eff_t;
	effect.Active = data.eff_a;

	std::vector<std::string> animationNames =
	{
		"Ходьба влево",
		"Ходьба вправо",
		"Покой влево",
		"Покой вправо",
	};

	for (int i = 0; i < (int)Sequence::StandingLeft; i++)
	{
		animations.emplace_back(Animation(data.anim_ps, data.anim_pe * i, data.anim_fw, data.anim_fh, data.anim_fa, image, data.anim_ft, animationNames[i], chromaKey));
	}
	for (int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(Animation(0, data.anim_pe * (i - (int)Sequence::StandingLeft), data.anim_fw, data.anim_fh, 1, image, data.anim_ft, animationNames[i], chromaKey));
	}
}

/* Главные методы для отрисовки главного персонажа */

void MainPerson::Draw(Graphics& gfx)
{
	if (effect.Active)
	{
		animations[(int)iCurSequence].DrawColor(DirectX::XMFLOAT2({ position.x - dx, position.y - dy }), wnd->Gfx(), Colors::Red);
	}
	else
	{
		animations[(int)iCurSequence].Draw(DirectX::XMFLOAT2({ position.x - dx, position.y - dy }), wnd->Gfx());
	}

	if (hitbox.IsVisible())
	{
		gfx.DrawHitBox(hitbox - DirectX::XMFLOAT2(dx, dy));
	}
}

void MainPerson::Process(float dt)
{
	DirectX::XMFLOAT2 dir = { 0.0f,0.0f };

	if (wnd->kbd.KeyIsPressed('W'))
	{
		if (AllowedMovingUp)
			dir.y -= 1.0f;
	}
	if (wnd->kbd.KeyIsPressed('A'))
	{
		if (AllowedMovingLeft)
			dir.x -= 1.0f;
	}
	if (wnd->kbd.KeyIsPressed('S') && !IsMovingDown && !IsOnJump)
	{
		if (AllowedMovingDown)
			dir.y += 1.0f;
	}
	if (wnd->kbd.KeyIsPressed('D'))
	{
		if (AllowedMovingRight)
			dir.x += 1.0f;
	}
	if (!IsOnJump && !IsMovingDown && wnd->kbd.KeyIsPressed(VK_SPACE) && AllowedMovingUp)
	{
		inertion = dir.x * speed * dt / 2.0f;
		IsOnJump = true;
	}

	if (!IsMovingDown && !IsOnJump)
	{
		inertion = 0.0f;
	}

	if (AllowedMovingDown && !IsOnJump && dir.y != -1.0f)
	{	
		float d = gravity * dt;

		if (cameraMode == CameraMode::SteadyPerson)
		{
			camera->Translate({ -inertion, (float)-d });
		}
		else if (cameraMode == CameraMode::SteadyWorld)
		{
			position.x += inertion;
			position.y += d;
			hitbox.Update(inertion, d);
		}
		else if (cameraMode == CameraMode::Hybrid)
		{
			//camera->Translate({ 0.0f, (float)-d / 1.5f });
			position.x += inertion;		
			position.y += d;
			hitbox.Update(inertion, d);
		}
	}

	SetDirection(dir);
	Update(dt);
}

void MainPerson::SetDirection(DirectX::XMFLOAT2 dir)
{
	if (dir.x > 0.0f)
	{
		iCurSequence = Sequence::WalkingRight;
	}
	else if (dir.x < 0.0f)
	{
		iCurSequence = Sequence::WalkingLeft;
	}
	else
	{
		if (vel.x > 0.0f)
		{
			iCurSequence = Sequence::StandingRight;
		}
		else if (vel.x < 0.0f)
		{
			iCurSequence = Sequence::StandingLeft;
		}
	}
	vel.x = dir.x * speed;
	vel.y = dir.y * speed;
}

void MainPerson::Update(float dt)
{
	vel.x *= dt;
	vel.y *= dt;

	if (cameraMode == CameraMode::SteadyPerson)
	{
		camera->Translate({ -vel.x, -vel.y });
	}
	else if (cameraMode == CameraMode::SteadyWorld)
	{
		position.x += vel.x;
		position.y += vel.y;
		hitbox.Update(vel.x, vel.y);
	}
	else if (cameraMode == CameraMode::Hybrid)
	{
		camera->Translate({ -vel.x / 1.5f, -vel.y / 1.5f});
		position.x += vel.x;
		position.y += vel.y;
		hitbox.Update(vel.x, vel.y);
	}
	
	if (IsOnJump)
	{
		if (jump_count > 0)
		{
			int d = (jump_count);
			d *= 60.0f * dt;

			if (cameraMode == CameraMode::SteadyPerson)
			{
				camera->Translate({ -inertion, (float)d });
			}
			else if (cameraMode == CameraMode::SteadyWorld)
			{
				position.x += inertion;
				position.y -= d;
				hitbox.Update(inertion, (float)-d);
			}
			else if (cameraMode == CameraMode::Hybrid)
			{
				//camera->Translate({ 0.0f, float(d) / 1.5f });
				position.x += inertion;
				position.y -= d;
				hitbox.Update(inertion, (float)-d);
			}
		}
		else
		{
			jump_count = jump_height;
			IsOnJump = false;
			IsMovingDown = true;
		}

		jump_count -= 1;
	}

	CalculateDeltas();

	animations[(int)iCurSequence].Update(dt);
	if (effect.Active)
	{
		effect.Time += dt;
		if (effect.Time >= effect.Duration)
		{
			effect.Active = false;
			effect.Time = 0.0f;
		}
	}
}

void MainPerson::ActivateEffect()
{
	effect.Active = true;
	effect.Time = 0.0f;
}

/***************************************************/

/* Главные методы для взаимодействия с главным персонажем */

void MainPerson::Translate(DirectX::XMFLOAT2 delta)
{
	position.x += delta.x;
	position.y += delta.y;

	hitbox.Translate(delta);
}

void MainPerson::SetAnimation(std::vector<Animation> anim)
{
	animations.clear();

	for (auto& a : anim)
	{
		animations.emplace_back(a);
	}
}

/**********************************************************/