#include "PhysicsEngine.h"

#include "Graphics.h"
#include "MainPerson.h"
#include "EngineFunctions.hpp"

void Physics::PhysicsEngine::LoadData(std::string dataPath)
{
	Clear();

	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о физике");
	}

	this->dataPath = dataPath;

	json j;
	dataFile >> j;

	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		if (d.find("line") != d.npos)
		{
			for (const auto& obj : j.at(d))
			{
				std::string name = d;
				float start_x = obj.at("start-x");
				float start_y = obj.at("start-y");
				float end_x = obj.at("end-x");
				float end_y = obj.at("end-y");

				AddLine(Line(name, start_x, start_y, end_x, end_y));
			}
		}
		else if (d.find("hb") != d.npos)
		{
			for (const auto& obj : j.at(d))
			{
				std::string name = d;
				float lt_x = obj.at("lt-x");
				float lt_y = obj.at("lt-y");
				float rb_x = obj.at("rb-x");
				float rb_y = obj.at("rb-y");

				AddHitBox(HitBox(name, lt_x, lt_y, rb_x, rb_y));
			}
		}
	}

	lines.shrink_to_fit();
	hitboxes.shrink_to_fit();
}

void Physics::PhysicsEngine::Clear()
{
	lines.clear();
	hitboxes.clear();
}

void Physics::PhysicsEngine::Draw(Graphics& gfx)
{
	for (auto& l : lines)
	{
		if (l.IsVisable())
		{
			gfx.DrawLine(l.GetStartPoint(), l.GetEndPoint(), Colors::Green);
		}
	}

	for (auto& hb : hitboxes)
	{
		gfx.DrawHitBox(hb);
	}
}

void Physics::PhysicsEngine::AddLine(Line line)
{
	lines.emplace_back(line);
}

void Physics::PhysicsEngine::AddLine(std::string name, float start_x, float start_y, float end_x, float end_y)
{
	AddLine(Line(name, start_x, start_y, end_x, end_y));
}

void Physics::PhysicsEngine::AddHitBox(HitBox hb)
{
	hitboxes.emplace_back(hb);
}

void Physics::PhysicsEngine::AddHitBox(std::string name, float leftTop_x, float leftTop_y, float rightBottom_x, float rightBottom_y)
{
	AddHitBox(HitBox(name, leftTop_x, leftTop_y, rightBottom_x, rightBottom_y));
}

void Physics::PhysicsEngine::UpdateLineAt(size_t k, Line line)
{
	lines.at(k) = line;
}

void Physics::PhysicsEngine::CheckMainPersonCollision(MainPerson* mp)
{
	auto mpHitBoxCoord = mp->GetHitBox();
	auto lns = GetLines(mpHitBoxCoord);

	if (CheckLineCollision(lns[0]))
	{
		mp->DisAllowMoveUp();
	}
	else
	{
		mp->AllowMoveUp();
	}

	if (CheckLineCollision(lns[1]))
	{
		mp->DisAllowMoveDown();
	}
	else
	{
		mp->AllowMoveDown();
	}

	if (CheckLineCollision(lns[2]))
	{
		mp->DisAllowMoveLeft();
	}
	else
	{
		mp->AllowMoveLeft();
	}

	if (CheckLineCollision(lns[3]))
	{
		mp->DisAllowMoveRight();
	}
	else
	{
		mp->AllowMoveRight();
	}
}

bool Physics::PhysicsEngine::CheckLineCollision(Line line)
{
	for (auto& l : lines)
	{
		if (line.IsIntersect(l))
		{
			return true;
			break;
		}
	}

	for (auto& hb : hitboxes)
	{
		auto lns = GetLines(hb);

		for (size_t i = 0; i < 4; i++)
		{
			if (line.IsIntersect(lns[i]))
			{
				return true;
				break;
			}
		}
	}

	return false;
}

std::vector<Physics::Line> Physics::GetLines(HitBox hb)
{
	auto hbCoord = hb.GetCoordinates();
	float d = 0.0f;

	std::vector<Physics::Line> lines =
	{
		Line(std::string("Line hitbox top"),    hbCoord.x - d, hbCoord.y - d, hbCoord.z + d, hbCoord.y - d),	// top
		Line(std::string("Line hitbox bottom"), hbCoord.x - d, hbCoord.w + d, hbCoord.z + d, hbCoord.w + d),	// bot
		Line(std::string("Line hitbox left"),   hbCoord.x - d, hbCoord.y - d, hbCoord.x - d, hbCoord.w + d),	// left
		Line(std::string("Line hitbox right"),  hbCoord.z + d, hbCoord.y + d, hbCoord.z + d, hbCoord.w + d),	// right
	};

	return lines;
}
