#include "Person.h"

Person::Person(std::string name,		 DirectX::XMFLOAT2 position, size_t layer,
			   std::string pathToSprite, HitBox			   hitbox,
			   float	   speed,		 float			   effectDuration,
			   float	   effectTime,	 bool			   effectActive)
	:
	Object2D(name, position, layer, pathToSprite),
	speed(speed),
	hitbox(hitbox)
{
	CalculateDeltas();

	effect.Duration = effectDuration;
	effect.Active = effectTime;
	effect.Time =   effectActive;

	for (int i = 0; i < (int)Sequence::StandingLeft; i++)
	{
		animations.emplace_back(Animation(90, 90 * i, 90, 90, 4, image, 0.16f));
	}
	for (int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(Animation(0, 90 * (i - (int)Sequence::StandingLeft), 90, 90, 1, image, 0.16f));
	}
}

/* Главные методы для отрисовки персонажа */

void Person::Draw(Graphics& gfx)
{
	if (effect.Active)
	{
		animations[(int)iCurSequence].DrawColor(DirectX::XMFLOAT2({ position.x - dx, position.y - dy }), gfx, Colors::Red);
	}
	else
	{
		animations[(int)iCurSequence].Draw(DirectX::XMFLOAT2({ position.x - dx, position.y - dy}), gfx);
	}

	if (hitbox.IsVisible())
	{
		gfx.DrawHitBox(hitbox - DirectX::XMFLOAT2(dx, dy));
	}
}

void Person::SetDirection(DirectX::XMFLOAT2 dir)
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

void Person::Update(float dt)
{
	vel.x *= dt;
	vel.y *= dt;

	position.x += vel.x;
	position.y += vel.y;

	hitbox.Update(vel.x, vel.y);
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

void Person::ActivateEffect()
{
	effect.Active = true;
	effect.Time = 0.0f;
}

/******************************************/

/* Главные методы для взаимодействия с персонажем */

void Person::Translate(DirectX::XMFLOAT2 delta)
{
	position.x += delta.x;
	position.y += delta.y;

	hitbox.Translate(delta);
}

void Person::SetHitBox(HitBox hb)
{
	hitbox = hb + DirectX::XMFLOAT2(dx, dy);
	CalculateDeltas();
}

HitBox Person::GetHitBox()
{
	return hitbox - DirectX::XMFLOAT2(dx, dy);
}

/**************************************************/

/* Внутренние методы */

void Person::CalculateDeltas()
{
	dx = hitbox.GetCoordinates().x - position.x;
	dy = hitbox.GetCoordinates().y - position.y;
}

/*********************/