#include "MainPerson.h"

MainPerson::MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera)
	:
	Object2D(data.name, data.position, data.layer, data.pathToSprite),
	dataPath(data.dataPath),
	speed(data.speed),
	jump_count(jump_height),
	holdTime(data.anim_ft),
	hitbox(data.name + std::string(" hitbox"), data.hb_coord),
	wnd(wnd),
	camera(camera),
	cameraMode(static_cast<CameraMode>(data.camMode))
{
	CalculateDeltas();

	effect.Duration = data.eff_d;
	effect.Time = data.eff_t;
	effect.Active = data.eff_a;

	std::vector<std::string> animationNames =
	{
		"Ходьба влево",
		"Ходьба вправо",
		"Ходьба вверх",
		"Ходьба вниз",
		"Покой влево",
		"Покой вправо",
		"Покой вверх",
		"Покой вниз",
	};

	for (int i = 0; i < (int)Sequence::StandingLeft; i++)
	{
		animations.emplace_back(Animation(data.anim_ps, data.anim_pe * i, data.anim_fw, data.anim_fh, data.anim_fa, image, data.anim_ft, animationNames[i]));
	}
	for (int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(Animation(0, data.anim_pe * (i - (int)Sequence::StandingLeft), data.anim_fw, data.anim_fh, 1, image, data.anim_ft, animationNames[i]));
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

void MainPerson::ProcessMoving(float dt)
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
	if (wnd->kbd.KeyIsPressed('S'))
	{
		if (AllowedMovingDown)
			dir.y += 1.0f;
	}
	if (wnd->kbd.KeyIsPressed('D'))
	{
		if (AllowedMovingRight)
			dir.x += 1.0f;
	}
	if (!IsOnJump && wnd->kbd.KeyIsPressed(VK_SPACE) && AllowedMovingUp)
	{
		IsOnJump = true;
	}

	if (AllowedMovingDown && !IsOnJump && dir.y != -1.0f)
	{
		float d = gravity * dt;
		
		if (cameraMode == CameraMode::SteadyPerson)
		{
			camera->Translate({ 0.0f, (float)d });
		}
		else if (cameraMode == CameraMode::SteadyWorld)
		{
			position.y += d;
			hitbox.UpdateY(d);
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
	else if (dir.y < 0.0f)
	{
		iCurSequence = Sequence::WalkingUp;
	}
	else if (dir.y > 0.0f)
	{
		iCurSequence = Sequence::WalkingDown;
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
		else if (vel.y < 0.0f)
		{
			iCurSequence = Sequence::StandingUp;
		}
		else if (vel.y > 0.0f)
		{
			iCurSequence = Sequence::StandingDown;
		}
	}
	vel.x = dir.x * speed;
	vel.y = dir.y * speed;
}

void MainPerson::Update(float dt)
{
	vel.x *= dt;
	vel.y *= dt;

	if (vel.x < 0) // Moving left
	{
		if (cameraMode == CameraMode::SteadyPerson)
		{
			camera->Translate({ -vel.x, 0.0f });
		}
		else if (cameraMode == CameraMode::SteadyWorld)
		{
			position.x += vel.x;
			hitbox.UpdateX(vel.x);
		}
	}
	else if (vel.x > 0)	// Moving right
	{
		if (cameraMode == CameraMode::SteadyPerson)
		{
			camera->Translate({ -vel.x, 0.0f });
		}
		else if (cameraMode == CameraMode::SteadyWorld)
		{
			position.x += vel.x;
			hitbox.UpdateX(vel.x);
		}
	}

	if (vel.y < 0) // Moving down
	{
		if (cameraMode == CameraMode::SteadyPerson)
		{
			camera->Translate({ 0.0f, -vel.y });
		}
		else if (cameraMode == CameraMode::SteadyWorld)
		{
			position.y += vel.y;
			hitbox.UpdateY(vel.y);
		}
	}
	else if (vel.y > 0)	// Moving up
	{
		if (cameraMode == CameraMode::SteadyPerson)
		{
			camera->Translate({ 0.0f, -vel.y });
		}
		else if (cameraMode == CameraMode::SteadyWorld)
		{
			position.y += vel.y;
			hitbox.UpdateY(vel.y);
		}	
	}

	if (IsOnJump)
	{
		if (jump_count >= -jump_height)
		{
			if (jump_count < 0)
			{
				int d = (jump_count * jump_count) / 2;
				position.y += d;
				hitbox.UpdateY((float)d);
			}
			else
			{
				int d = (jump_count * jump_count) / 2;
				position.y -= d;	
				hitbox.UpdateY((float)-d);
			}

			jump_count -= 1;
		}
		else
		{
			IsOnJump = false;
			jump_count = jump_height;
		}
	}

	CalculateDeltas();

	animations[(int)iCurSequence].Update(dt);
	// update effect time if active
	if (effect.Active)
	{
		effect.Time += dt;
		// deactivate effect if duration exceeded
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

void MainPerson::AllowMoveUp()
{
	AllowedMovingUp = true;
}

void MainPerson::AllowMoveDown()
{
	AllowedMovingDown = true;
}

void MainPerson::AllowMoveLeft()
{
	AllowedMovingLeft = true;
}

void MainPerson::AllowMoveRight()
{
	AllowedMovingRight = true;
}

void MainPerson::DisAllowMoveUp()
{
	AllowedMovingUp = false;
}

void MainPerson::DisAllowMoveDown()
{
	AllowedMovingDown = false;
}

void MainPerson::DisAllowMoveLeft()
{
	AllowedMovingLeft = false;
}

void MainPerson::DisAllowMoveRight()
{
	AllowedMovingRight = false;
}

void MainPerson::SetHitBox(HitBox hb)
{
	hitbox = hb + DirectX::XMFLOAT2(dx, dy);
	CalculateDeltas();
}

HitBox MainPerson::GetHitBox()
{
	return hitbox - DirectX::XMFLOAT2(dx, dy);
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

/* Внутренние методы */

void MainPerson::CalculateDeltas()
{
	dx = hitbox.GetCoordinates().x - position.x;
	dy = hitbox.GetCoordinates().y - position.y;
}

/*********************/