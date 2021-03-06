#include "PhysicObject2D.h"

PhysicObject2D::PhysicObject2D(std::string name, DirectX::XMFLOAT2 position, HitBox	hitbox, size_t layer, std::string pathToSprite,
							   Color key, bool chromaKeyAble, float speed, int jump_height, float gravity)
	:
	Object2D(name, position, layer, pathToSprite, key, chromaKeyAble),
	jump_height(jump_height),
	jump_count(jump_height),
	gravity(gravity),
	hitbox(hitbox),
	speed(speed)
{
	CalculateDeltas();
}

void PhysicObject2D::AllowMoveUp()
{
	AllowedMovingUp = true;
}

void PhysicObject2D::AllowMoveDown()
{
	AllowedMovingDown = true;
}

void PhysicObject2D::AllowMoveLeft()
{
	AllowedMovingLeft = true;
}

void PhysicObject2D::AllowMoveRight()
{
	AllowedMovingRight = true;
}

void PhysicObject2D::DisAllowMoveUp()
{
	AllowedMovingUp = false;
}

void PhysicObject2D::DisAllowMoveDown()
{
	AllowedMovingDown = false;
	IsMovingDown = false;
}

void PhysicObject2D::DisAllowMoveLeft()
{
	AllowedMovingLeft = false;
}

void PhysicObject2D::DisAllowMoveRight()
{
	AllowedMovingRight = false;
}

/* ?????????? ?????? */

void PhysicObject2D::Translate(DirectX::XMFLOAT2 delta)
{
	position.x += delta.x;
	position.y += delta.y;

	hitbox.Translate(delta);
}

void PhysicObject2D::SetHitBox(HitBox hb)
{
	hitbox = hb + DirectX::XMFLOAT2(dx, dy);
	CalculateDeltas();
}

HitBox PhysicObject2D::GetHitBox()
{
	return hitbox - DirectX::XMFLOAT2(dx, dy);
}

void PhysicObject2D::CalculateDeltas()
{
	dx = hitbox.GetCoordinates().x - position.x;
	dy = hitbox.GetCoordinates().y - position.y;
}

int PhysicObject2D::GetDirection()
{
	if (iCurSequence == Sequence::StandingLeft || iCurSequence == Sequence::WalkingLeft)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

/*********************/