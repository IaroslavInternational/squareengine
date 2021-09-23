#pragma once

#include <DirectXMath.h>
#include "Colors.h"

class HitBox
{
public:
	HitBox(DirectX::XMFLOAT4 coordinates, Color color = Colors::DodgerBlue);
	HitBox(float leftTopX,     float leftTopY,
		   float rightBottomX, float rightBottomY,
		   Color color = Colors::DodgerBlue);
public:
	HitBox operator-(DirectX::XMFLOAT2 delta);
	HitBox operator+(DirectX::XMFLOAT2 delta);
public:
	void Update(DirectX::XMFLOAT2 delta);
	void Update(float dx, float dy);

	DirectX::XMFLOAT4& GetCoordinates();
	
	void SetColor(Color color);
	Color& GetColor(Color color);
public:
	bool IsCollide(HitBox& hb);
private:
	DirectX::XMFLOAT4 coordinates;
	Color color;
};