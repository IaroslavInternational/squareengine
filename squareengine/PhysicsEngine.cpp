#include "PhysicsEngine.h"

#include "Graphics.h"
#include "PhysicObject2D.h"
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
		else if (d.find("settings") != d.npos)
		{
			for (const auto& obj : j.at(d))
			{
				lineColor[0] = obj.at("l-clr-r");
				lineColor[1] = obj.at("l-clr-g");
				lineColor[2] = obj.at("l-clr-b");

				hbColor[0] = obj.at("h-clr-r");
				hbColor[1] = obj.at("h-clr-g");
				hbColor[2] = obj.at("h-clr-b");

				deltaCollision = obj.at("delta-collision");
				objVisability = obj.at("obj-vis");
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
	if (objVisability)
	{
		for (auto& l : lines)
		{
			if (l.IsVisible())
			{
				gfx.DrawLine(l.GetStartPoint(), l.GetEndPoint(), Color(char(lineColor[0] * 255.0f), char(lineColor[1] * 255.0f), char(lineColor[2] * 255.0f)));
			}
		}

		for (auto& hb : hitboxes)
		{
			if (hb.IsVisible())
			{
				gfx.DrawHitBox(hb, Color(char(hbColor[0] * 255.0f), char(hbColor[1] * 255.0f), char(hbColor[2] * 255.0f)));
			}
		}
	}
}

void Physics::PhysicsEngine::TranslateObjects(DirectX::XMFLOAT2 delta)
{
	for (auto& l : lines)
	{
		l.Translate(delta);
	}

	for (auto& hb : hitboxes)
	{
		hb.Translate(delta);
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

void Physics::PhysicsEngine::DeleteLineAt(size_t k)
{
	DeleteLineAt(lines.begin() + k);
}

void Physics::PhysicsEngine::DeleteLineAt(std::vector<Line>::const_iterator it)
{
	lines.erase(it);
}

void Physics::PhysicsEngine::DeleteHitBoxAt(size_t k)
{
	DeleteHitBoxAt(hitboxes.begin() + k);
}

void Physics::PhysicsEngine::DeleteHitBoxAt(std::vector<HitBox>::const_iterator it)
{
	hitboxes.erase(it);
}

void Physics::PhysicsEngine::UpdateHitBoxAt(size_t k, HitBox hb)
{
	hitboxes.at(k) = hb;
}

size_t Physics::PhysicsEngine::GetLinesAmount()
{
	return lines.size();
}

size_t Physics::PhysicsEngine::GetHitBoxAmount()
{
	return hitboxes.size();
}

void Physics::PhysicsEngine::CheckCollision(PhysicObject2D* obj)
{
	auto lns = GetLines(obj->GetHitBox());

	if (CheckLineCollision(lns[0]))
	{
		obj->DisAllowMoveUp();
	}
	else
	{
		obj->AllowMoveUp();
	}

	if (CheckLineCollision(lns[1]))
	{
		obj->DisAllowMoveDown();
	}
	else
	{
		obj->AllowMoveDown();
	}

	if (CheckLineCollision(lns[2]))
	{
		obj->DisAllowMoveLeft();		
	}
	else
	{
		obj->AllowMoveLeft();
	}

	if (CheckLineCollision(lns[3]))
	{
		obj->DisAllowMoveRight();
	}
	else
	{
		obj->AllowMoveRight();
	}

	CheckOverlapping(obj);
}

bool Physics::PhysicsEngine::CheckLineCollision(Line line)
{
	for (auto& l : lines)
	{
		if (line.IsIntersect(l))
		{
			return true;
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
			}
		}
	}

	return false;
}

void Physics::PhysicsEngine::CheckOverlapping(PhysicObject2D* obj)
{
	DirectX::XMFLOAT2 delta;
	
	for (auto& hb : hitboxes)
	{
		if (obj->GetHitBox().IsCollide(hb))
		{
			// Проверка движения вправо
			if (obj->GetDirection() == 1)
			{
				// Если произошло наложение хитбоков 
				if (hb.GetCoordinates().x < obj->GetHitBox().GetCoordinates().z &&
					hb.GetCoordinates().y < obj->GetHitBox().GetCoordinates().w)
				{
					delta.x = obj->GetHitBox().GetCoordinates().z - hb.GetCoordinates().x;
					delta.y = obj->GetHitBox().GetCoordinates().w - hb.GetCoordinates().y;

					// Выталкнуть хитбокс вверх
					if (delta.x > delta.y)
					{
						obj->Translate({ 0.0f, -delta.y - 2.0f });
						break;
					}
					else if (delta.x < delta.y) // Выталкнуть хитбокс влево
					{
						obj->Translate({ -delta.x - 2.0f, 0.0f });
						break;
					}
				}
			}
			else // Проверка движения влево
			{
				// Если произошло наложение хитбоков 
				if (hb.GetCoordinates().z > obj->GetHitBox().GetCoordinates().x &&
					hb.GetCoordinates().y < obj->GetHitBox().GetCoordinates().w)
				{
					delta.x = hb.GetCoordinates().z - obj->GetHitBox().GetCoordinates().x;
					delta.y = obj->GetHitBox().GetCoordinates().w - hb.GetCoordinates().y;

					// Выталкнуть хитбокс вверх
					if (delta.x > delta.y)
					{
						obj->Translate({ 0.0f, -delta.y - 2.0f });
						break;
					}
					else if (delta.x < delta.y) // Выталкнуть хитбокс вправо
					{
						obj->Translate({ delta.x + 2.0f, 0.0f });
						break;
					}
				}
			}
		}
	}
}

std::vector<Physics::Line> Physics::PhysicsEngine::GetLines(HitBox hb)
{
	auto& hbCoord = hb.GetCoordinates();

	return std::vector<Physics::Line>
	{
		Line(std::string("L hb t"), hbCoord.x - deltaCollision, hbCoord.y - deltaCollision, hbCoord.z + deltaCollision, hbCoord.y - deltaCollision), // top
		Line(std::string("L hb b"), hbCoord.x - deltaCollision, hbCoord.w + deltaCollision, hbCoord.z + deltaCollision, hbCoord.w + deltaCollision), // bot
		Line(std::string("L hb l"), hbCoord.x - deltaCollision, hbCoord.y - deltaCollision, hbCoord.x - deltaCollision, hbCoord.w + deltaCollision), // left
		Line(std::string("L hb r"), hbCoord.z + deltaCollision, hbCoord.y + deltaCollision, hbCoord.z + deltaCollision, hbCoord.w + deltaCollision), // right
	};
}