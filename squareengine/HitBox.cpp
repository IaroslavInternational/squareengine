#include "HitBox.h"

HitBox::HitBox(DirectX::XMFLOAT4 coordinates, Color color)
	:
	coordinates(coordinates),
	color(color)
{}

HitBox::HitBox(float   leftTopX,     float leftTopY,
			   float   rightBottomX, float rightBottomY,
			   Color color)
	:
	HitBox(DirectX::XMFLOAT4(leftTopX, leftTopY, rightBottomX, rightBottomY), color)
{}

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

DirectX::XMFLOAT4& HitBox::GetCoordinates()
{
	return coordinates;
}

void HitBox::SetColor(Color color)
{
	this->color = color;
}

Color& HitBox::GetColor(Color color)
{
	return color;
}

bool HitBox::IsCollide(HitBox& hb)
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
