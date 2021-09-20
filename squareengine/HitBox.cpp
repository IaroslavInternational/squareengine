#include "HitBox.h"

HitBox::HitBox(DirectX::XMINT4 coordinates)
	:
	coordinates(coordinates)
{}

HitBox::HitBox(int leftTopX,     int leftTopY,
			   int rightBottomX, int rightBottomY)
	:
	HitBox(DirectX::XMINT4{ leftTopX, leftTopY, rightBottomX, rightBottomY })
{}

DirectX::XMINT4& HitBox::GetCoordinates()
{
	return coordinates;
}
