#pragma once

#include <DirectXMath.h>

class HitBox
{
public:
	HitBox(DirectX::XMINT4 coordinates);
	HitBox(int leftTopX,     int leftTopY,
		   int rightBottomX, int rightBottomY);
public:
	HitBox operator-(DirectX::XMINT2 delta);
public:
	DirectX::XMINT4& GetCoordinates();
private:
	DirectX::XMINT4 coordinates;
};