#include "PhysicObject2D.h"

PhysicObject2D::PhysicObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite,
							   int jump_height, float gravity)
	:
	Object2D(name, position, layer, pathToSprite),
	jump_height(jump_height),
	jump_count(jump_height),
	gravity(gravity)
{}

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