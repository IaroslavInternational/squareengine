#include "MainPerson.h"

MainPerson::MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera)
	:
	Object2D(data.name, data.position, data.pathToSprite, HitBox(data.name + std::string(" hitbox"), data.hb_coord)),
	dataPath(data.dataPath),
	speed(data.speed),
	wnd(wnd),
	camera(camera)
{
	effect.Duration = data.eff_d;
	effect.Time = data.eff_t;
	effect.Active = data.eff_a;

	for (int i = 0; i < (int)Sequence::StandingLeft; i++)
	{
		animations.emplace_back(Animation(90, 90 * i, 90, 90, 4, sprite, 0.16f));
	}
	for (int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(Animation(0, 90 * (i - (int)Sequence::StandingLeft), 90, 90, 1, sprite, 0.16f));
	}
}

/* Главные методы для отрисовки главного персонажа */

void MainPerson::Draw()
{
	if (effect.Active)
	{
		animations[(int)iCurSequence].DrawColor(DirectX::XMFLOAT2({ position.x - dx, position.y - dy }), wnd->Gfx(), Colors::Red);
	}
	else
	{
		animations[(int)iCurSequence].Draw(DirectX::XMFLOAT2({ position.x - dx, position.y - dy }), wnd->Gfx());
	}

	if (hitbox_visability)
	{
		wnd->Gfx().DrawHitBox(hitbox - DirectX::XMFLOAT2(dx, dy));
	}
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
		//position.x += vel.x;
		//hitbox.UpdateX(vel.x);

		camera->Translate({ -vel.x, 0.0f });
	}
	else if (vel.x > 0)	// Moving right
	{
		//position.x += vel.x;
		//hitbox.UpdateX(vel.x);

		camera->Translate({ -vel.x, 0.0f });
	}

	if (vel.y < 0) // Moving down
	{
		//position.y += vel.y;
		//hitbox.UpdateY(vel.y);
		
		camera->Translate({ 0.0f, -vel.y });
	}
	else if (vel.y > 0)	// Moving up
	{
		//position.y += vel.y;
		//hitbox.UpdateY(vel.y);

		camera->Translate({ 0.0f, -vel.y });
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

void MainPerson::ProcessMoving(float dt)
{
	DirectX::XMFLOAT2 dir = { 0.0f,0.0f };
	if (!wnd->CursorEnabled())
	{
		if (wnd->kbd.KeyIsPressed('W'))
		{
			if(AllowedMovingUp)
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
	}

	SetDirection(dir);
	Update(dt);
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

/**********************************************************/