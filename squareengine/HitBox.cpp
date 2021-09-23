#include "HitBox.h"

HitBox::HitBox(DirectX::XMINT4 coordinates)
	:
	coordinates(coordinates)
{}

HitBox::HitBox(int leftTopX,     int leftTopY,
			   int rightBottomX, int rightBottomY)
	:
	HitBox(DirectX::XMINT4(leftTopX, leftTopY, rightBottomX, rightBottomY))
{}

HitBox HitBox::operator-(DirectX::XMINT2 delta)
{
	return HitBox(coordinates.x - delta.x, coordinates.y - delta.y,
				  coordinates.z - delta.x, coordinates.w - delta.y);
}

HitBox HitBox::operator+(DirectX::XMINT2 delta)
{
	return HitBox(coordinates.x + delta.x, coordinates.y + delta.y,
				  coordinates.z + delta.x, coordinates.w + delta.y);
}

void HitBox::Update(DirectX::XMINT2 delta)
{
	coordinates.x += delta.x;
	coordinates.y += delta.y;
	coordinates.z += delta.x;
	coordinates.w += delta.y;
}

void HitBox::Update(int dx, int dy)
{
	Update(DirectX::XMINT2(dx, dy));
}

DirectX::XMINT4& HitBox::GetCoordinates()
{
	return coordinates;
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
