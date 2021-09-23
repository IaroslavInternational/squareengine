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
	HitBox operator+(DirectX::XMINT2 delta);
public:
	void Update(DirectX::XMINT2 delta);
	void Update(int dx, int dy);

	DirectX::XMINT4& GetCoordinates();
public:
	bool IsCollide(HitBox& hb);
private:
	DirectX::XMINT4 coordinates;
};