#include "HitBox.h"

HitBox::HitBox(DirectX::XMFLOAT4 coordinates)
	:
	coordinates(coordinates)
{}

HitBox::HitBox(float   leftTopX,     float leftTopY,
			   float   rightBottomX, float rightBottomY)
	:
	HitBox(DirectX::XMFLOAT4(leftTopX, leftTopY, rightBottomX, rightBottomY))
{}

/* Операторы */

HitBox HitBox::operator-(DirectX::XMFLOAT2 delta)
{
	return HitBox(coordinates.x - delta.x, coordinates.y - delta.y,
				  coordinates.z - delta.x, coordinates.w - delta.y);
}

HitBox HitBox::operator+(DirectX::XMFLOAT2 delta)
{
	return HitBox(coordinates.x + delta.x, coordinates.y + delta.y,
				  coordinates.z + delta.x, coordinates.w + delta.y);
}

/*************/

/* Основные методы для работы с hitbox */

void HitBox::Update(DirectX::XMFLOAT2 delta)
{
	coordinates.x += delta.x;
	coordinates.y += delta.y;
	coordinates.z += delta.x;
	coordinates.w += delta.y;
}

void HitBox::Update(float dx, float dy)
{
	Update(DirectX::XMFLOAT2(dx, dy));
}

void HitBox::UpdateX(float dx)
{
	coordinates.x += dx;
	coordinates.z += dx;
}

void HitBox::UpdateY(float dy)
{
	coordinates.y += dy;
	coordinates.w += dy;
}

DirectX::XMFLOAT4& HitBox::GetCoordinates()
{
	return coordinates;
}

/***************************************/

/* Методы операций с hitbox */

bool HitBox::IsCollide(HitBox hb)
{
    return ((((coordinates.x >= hb.coordinates.x &&	   coordinates.x <= hb.coordinates.z)		||
			  (coordinates.z >= hb.coordinates.x &&	   coordinates.z <= hb.coordinates.z))		&& 
			 ((coordinates.y >= hb.coordinates.y &&	   coordinates.y <= hb.coordinates.w)		|| 
			  (coordinates.w >= hb.coordinates.y &&	   coordinates.w <= hb.coordinates.w)))		||
		 (((hb.coordinates.x >=	   coordinates.x && hb.coordinates.x <=	   coordinates.z)		|| 
		   (hb.coordinates.z >=	   coordinates.x && hb.coordinates.z <=	   coordinates.z))		&&
		  ((hb.coordinates.y >=	   coordinates.y && hb.coordinates.y <=	   coordinates.w)		|| 
		   (hb.coordinates.w >=	   coordinates.y && hb.coordinates.w <=	   coordinates.w))))	||
		   ((((coordinates.x >= hb.coordinates.x &&    coordinates.x <= hb.coordinates.z)		|| 
			  (coordinates.z >= hb.coordinates.x &&    coordinates.z <= hb.coordinates.z))		&& 
		  ((hb.coordinates.y >=	   coordinates.y && hb.coordinates.y <=	   coordinates.w)		||
		   (hb.coordinates.w >=	   coordinates.y && hb.coordinates.w <=	   coordinates.w)))		||
		 (((hb.coordinates.x >=	   coordinates.x && hb.coordinates.x <=	   coordinates.z)		|| 
		   (hb.coordinates.z >=	   coordinates.x && hb.coordinates.z <=	   coordinates.z))		&&
			 ((coordinates.y >= hb.coordinates.y &&	   coordinates.y <= hb.coordinates.w)		|| 
			  (coordinates.w >= hb.coordinates.y &&	   coordinates.w <= hb.coordinates.w))));	
}

/****************************/