#include "HitBox.h"

HitBox::HitBox(std::string name, DirectX::XMFLOAT4 coordinates)
	:
	name(name),
	coordinates(coordinates)
{}

HitBox::HitBox(std::string name, float   leftTopX,     float leftTopY,
								 float   rightBottomX, float rightBottomY)
	:
	HitBox(name, DirectX::XMFLOAT4(leftTopX, leftTopY, rightBottomX, rightBottomY))
{}

/* ????????? */

void HitBox::operator=(HitBox hb)
{
	name = hb.name;
	coordinates = hb.coordinates;
	visability = hb.visability;
}

HitBox HitBox::operator-(DirectX::XMFLOAT2 delta)
{
	return HitBox(name, coordinates.x - delta.x, coordinates.y - delta.y,
						coordinates.z - delta.x, coordinates.w - delta.y);
}

HitBox HitBox::operator+(DirectX::XMFLOAT2 delta)
{
	return HitBox(name, coordinates.x + delta.x, coordinates.y + delta.y,
					    coordinates.z + delta.x, coordinates.w + delta.y);
}

/*************/

/* ???????? ?????? ??? ?????? ? hitbox */

void HitBox::Translate(DirectX::XMFLOAT2 delta)
{
	coordinates.x += delta.x;
	coordinates.y += delta.y;
	coordinates.z += delta.x;
	coordinates.w += delta.y;
}

bool HitBox::IsVisible()
{
	return visability;
}

void HitBox::SetVisibility(bool state)
{
	visability = state;
}

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

void HitBox::UpdateLeftSide(float dx)
{
	coordinates.x -= dx;
}

void HitBox::UpdateRightSide(float dz)
{
	coordinates.z += dz;
}

std::string HitBox::GetName()
{
	return name;
}

DirectX::XMFLOAT4& HitBox::GetCoordinates()
{
	return coordinates;
}

/***************************************/

/* ?????? ???????? ? hitbox */

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

bool HitBox::IsOverlap(DirectX::XMFLOAT2 point)
{
	return  (point.x >= coordinates.x && point.x <= coordinates.z) &&
			(point.y >= coordinates.y && point.y <= coordinates.w);
}

/****************************/