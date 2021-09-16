#include "Object2D.h"

Object2D::Object2D(std::string name, DirectX::XMFLOAT2 position)
	:
	name(name),
	position(position)
{}

std::string Object2D::GetName() const
{
	return name;
}

DirectX::XMFLOAT2 Object2D::GetPosition()
{
	return position;
}

void Object2D::SetName(std::string name)
{
	this->name = name;
}

void Object2D::SetPosition(DirectX::XMFLOAT2 position)
{
	this->position = position;
}
