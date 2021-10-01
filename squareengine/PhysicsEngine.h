#pragma once

#include "Line.h"
#include "HitBox.h"

#include <vector>
#include <string>

class Graphics;
class MainPerson;

namespace Physics
{
	std::vector<Physics::Line> GetLines(HitBox hb);

	class PhysicsEngine
	{
	public:
		PhysicsEngine() = default;
	public:
		void LoadData(std::string dataPath);
		void Clear();
		void Draw(Graphics& gfx);
	public:
		void AddLine(Line line);
		void AddLine(float start_x, float start_y, float end_x, float end_y);
		void AddHitBox(HitBox hb);
		void AddHitBox(float leftTop_x, float leftTop_y, float rightBottom_x, float rightBottom_y);
	public:
		void CheckMainPersonCollision(MainPerson* mp);
		bool CheckLineCollision(Line line);
	private:
		std::vector<Line>	lines;
		std::vector<HitBox> hitboxes;
	};
}