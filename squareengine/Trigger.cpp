#include "Trigger.h"

Trigger::Trigger(float start_x, float start_y, float end_x, float end_y)
	:
	Trigger({ start_x, start_y }, { end_x, end_y })
{}

Trigger::Trigger(const DirectX::XMFLOAT2& start, const DirectX::XMFLOAT2& end)
	:
	start(start),
	end(end)
{
}

bool Trigger::IsCollide(const HitBox& hitbox)
{
	return false;
}

bool Trigger::IsCollide(const Physics::Line& line)
{
	return false;
}
