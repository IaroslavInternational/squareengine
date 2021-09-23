#pragma once

#include <DirectXMath.h>
#include "Colors.h"

class HitBox
{
public:
	HitBox(DirectX::XMINT4 coordinates, Color color = Colors::DodgerBlue);
	HitBox(int	 leftTopX,     int leftTopY,
		   int	 rightBottomX, int rightBottomY,
		   Color color = Colors::DodgerBlue);
public:
	HitBox operator-(DirectX::XMINT2 delta);
	HitBox operator+(DirectX::XMINT2 delta);
public:
	void Update(DirectX::XMINT2 delta);
	void Update(int dx, int dy);

	DirectX::XMINT4& GetCoordinates();
	
	void SetColor(Color color);
	Color& GetColor(Color color);
public:
	bool IsCollide(HitBox& hb);
private:
	DirectX::XMINT4 coordinates;
	Color color;
};