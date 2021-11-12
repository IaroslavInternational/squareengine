#include "Trigger.h"

Trigger::Trigger(float start_x, float start_y, float end_x, float end_y)
	:
	Trigger({ start_x, start_y }, { end_x, end_y })
{}

Trigger::Trigger(const DirectX::XMFLOAT2& start, const DirectX::XMFLOAT2& end)
	:
	Trigger(Physics::Line("trigger line", start, end))
{
}

Trigger::Trigger(const Physics::Line& line)
	:
	line(line)
{}

bool Trigger::IsCollide(HitBox& hitbox)
{
	auto lines = GetLines(hitbox);

	for (auto& l : lines)
	{
		if (line.IsIntersect(l))
		{
			return true;
		}
	}

	return false;
}

bool Trigger::IsCollide(const Physics::Line& line)
{
	return this->line.IsIntersect(line);
}

std::vector<Physics::Line> Trigger::GetLines(HitBox& hitbox)
{
	auto hbCoord = hitbox.GetCoordinates();

	std::vector<Physics::Line> lines =
	{
		Physics::Line(std::string("Line hitbox top"),    hbCoord.x, hbCoord.y, hbCoord.z, hbCoord.y),	// top
		Physics::Line(std::string("Line hitbox bottom"), hbCoord.x, hbCoord.w, hbCoord.z, hbCoord.w),	// bot
		Physics::Line(std::string("Line hitbox left"),   hbCoord.x, hbCoord.y, hbCoord.x, hbCoord.w),	// left
		Physics::Line(std::string("Line hitbox right"),  hbCoord.z, hbCoord.y, hbCoord.z, hbCoord.w),	// right
	};

	return lines;
}