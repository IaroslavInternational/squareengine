#include "Object2D.h"

Object2D::Object2D(std::string name, DirectX::XMFLOAT2 position, std::string pathToSprite, HitBox hitbox)
	:
	name(name),
	position(position),
	sprite(pathToSprite),
	hitbox(hitbox)
{
	CalculateDeltas();
}

/* Главные методы для описания объекта */

void Object2D::SetName(std::string name)
{
	this->name = name;
}

void Object2D::SetPosition(DirectX::XMFLOAT2 position)
{
	this->position = position;
}

void Object2D::SetSurface(Surface2D sprite)
{
	this->sprite = sprite;
}

void Object2D::SetHitBox(HitBox hb)
{
	hitbox = hb + DirectX::XMFLOAT2(dx, dy);
	CalculateDeltas();
}

std::string Object2D::GetName() const
{
	return name;
}

DirectX::XMFLOAT2 Object2D::GetPosition()
{
	return position;
}

Surface2D& Object2D::GetSurface()
{
	return sprite;
}

HitBox Object2D::GetHitBox()
{
	return hitbox - DirectX::XMFLOAT2(dx, dy);
}

/***************************************/

/* Внутренние методы */

void Object2D::CalculateDeltas()
{
	dx = hitbox.GetCoordinates().x - position.x;
	dy = hitbox.GetCoordinates().y - position.y;
}

/*********************/