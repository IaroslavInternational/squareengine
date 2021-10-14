#include "Object2D.h"

Object2D::Object2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToimage)
	:
	name(name),
	position(position),
	layer(layer),
	image(pathToimage)
{
}

/* Главные методы для описания объекта */

void Object2D::Translate(DirectX::XMFLOAT2 delta)
{
	position.x += delta.x;
	position.y += delta.y;
}

void Object2D::SetName(std::string name)
{
	this->name = name;
}

void Object2D::SetPosition(DirectX::XMFLOAT2 position)
{
	this->position = position;
}

void Object2D::SetSurface(Surface2D image)
{
	this->image = image;
}

void Object2D::LayerUp()
{
	layer++;
}

void Object2D::LayerDown()
{
	layer--;
}

std::string Object2D::GetName() const
{
	return name;
}

DirectX::XMFLOAT2 Object2D::GetPosition()
{
	return position;
}

size_t Object2D::GetLayer()
{
	return layer;
}

Surface2D& Object2D::GetSurface()
{
	return image;
}

/***************************************/
