#pragma once

#include "TriggerType.hpp"
#include "HitBox.h"
#include "Line.h"

class Trigger
{
public:
	Trigger(float start_x, float start_y, float end_x, float end_y);
	Trigger(const DirectX::XMFLOAT2& start, const DirectX::XMFLOAT2& end);
public:
	bool IsCollide(const HitBox& hitbox);
	bool IsCollide(const Physics::Line& line);
private:
	DirectX::XMFLOAT2 start;
	DirectX::XMFLOAT2 end;
};