#include "Character.h"

Character::Character(const std::string& path, DirectX::XMFLOAT2 coordinates,
	float speed, float effectDuration, float effectTime, bool effectActive)
	:
	Object(std::string("Test"), DirectX::XMFLOAT3({ coordinates.x, coordinates.y, 0.0f }), DirectX::XMFLOAT3({0.0f, 0.0f, 0.0f})),
	sprite(path),
	speed(speed),
	effectDuration(effectDuration),
	effectTime(effectTime),
	effectActive(effectActive)
{
	for (int i = 0; i < (int)Sequence::StandingLeft; i++)
	{
		animations.emplace_back(Animation(90, 90 * i, 90, 90, 4, sprite, 0.16f));
	}
	for (int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(Animation(0, 90 * (i - (int)Sequence::StandingLeft), 90, 90, 1, sprite, 0.16f));
	}
}

void Character::Draw(Graphics& gfx)
{
	// if effect active, draw sprite replacing opaque pixels with red
	if( effectActive )
	{
		animations[(int)iCurSequence].DrawColor(DirectX::XMFLOAT2({ position.x, position.y }), gfx, Colors::Red);
	}
	else
	{
		animations[(int)iCurSequence].Draw(DirectX::XMFLOAT2({ position.x, position.y }), gfx);
	}
}

void Character::SetDirection(DirectX::XMFLOAT2 dir)
{
	if( dir.x > 0.0f )
	{
		iCurSequence = Sequence::WalkingRight;
	}
	else if( dir.x < 0.0f )
	{
		iCurSequence = Sequence::WalkingLeft;
	}
	else if( dir.y < 0.0f )
	{
		iCurSequence = Sequence::WalkingUp;
	}
	else if( dir.y > 0.0f )
	{
		iCurSequence = Sequence::WalkingDown;
	}
	else
	{
		if( vel.x > 0.0f )
		{
			iCurSequence = Sequence::StandingRight;
		}
		else if( vel.x < 0.0f )
		{
			iCurSequence = Sequence::StandingLeft;
		}
		else if( vel.y < 0.0f )
		{
			iCurSequence = Sequence::StandingUp;
		}
		else if( vel.y > 0.0f )
		{
			iCurSequence = Sequence::StandingDown;
		}
	}
	vel.x = dir.x * speed;
	vel.y = dir.y * speed;
}

void Character::Update(float dt)
{
	vel.x *= dt;
	vel.y *= dt;
	
	position.x += vel.x;
	position.y += vel.y;

	animations[(int)iCurSequence].Update(dt);
	// update effect time if active
	if (effectActive)
	{
		effectTime += dt;
		// deactivate effect if duration exceeded
		if (effectTime >= effectDuration)
		{
			effectActive = false;
		}
	}
}

void Character::ActivateEffect()
{
	effectActive = true;
	effectTime = 0.0f;
}
