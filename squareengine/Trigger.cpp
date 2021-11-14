#include "Trigger.h"
#include "Graphics.h"

Trigger::Trigger(std::string name, float start_x, float start_y, float end_x, float end_y, TriggerType type, std::string goal)
	:
	Trigger(name, { start_x, start_y }, { end_x, end_y }, type, goal)
{}

Trigger::Trigger(std::string name, const DirectX::XMFLOAT2& start, const DirectX::XMFLOAT2& end, TriggerType type, std::string goal)
	:
	Trigger(name, Physics::Line(name + std::string("line"), start, end), type, goal)
{
}

Trigger::Trigger(std::string name, const Physics::Line& line, TriggerType type, std::string goal)
	:
	name(name),
	line(line),
	type(type),
	goal(goal)
{}

void Trigger::Translate(const DirectX::XMFLOAT2& delta)
{
	line.Translate(delta);
}

void Trigger::Draw(Graphics& gfx)
{
	gfx.DrawLine(line, Colors::Yellow);
}

bool Trigger::IsCollide(HitBox hitbox)
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

TriggerType Trigger::GetType() const
{
	return type;
}

std::string Trigger::GetName() const
{
	return name;
}

std::string Trigger::GetGoal() const
{
	return goal;
}

Physics::Line Trigger::GetLine() const
{
	return line;
}

void Trigger::SetLine(Physics::Line line)
{
	this->line = line;
}

std::vector<Physics::Line> Trigger::GetLines(HitBox hitbox)
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