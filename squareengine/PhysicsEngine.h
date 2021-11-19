#pragma once

#include "Line.h"
#include "HitBox.h"

#include <vector>
#include <string>

class Graphics;
class PhysicObject2D;

namespace Physics
{
	class PhysicsEngine
	{
	public:
		friend class GUISystem;
	public:
		PhysicsEngine() = default;
	public:
		void LoadData(std::string dataPath);
		void Clear();
		void Draw(Graphics& gfx);
		void TranslateObjects(DirectX::XMFLOAT2 delta);
	public:
		void   AddLine(Line line);
		void   AddLine(std::string name, float start_x, float start_y, float end_x, float end_y);
		void   AddHitBox(HitBox hb);
		void   AddHitBox(std::string name, float leftTop_x, float leftTop_y, float rightBottom_x, float rightBottom_y);
		void   UpdateLineAt(size_t k, Line line);
		void   DeleteLineAt(size_t k);
		void   DeleteLineAt(std::vector<Line>::const_iterator it);
		void   UpdateHitBoxAt(size_t k, HitBox hb);
		void   DeleteHitBoxAt(size_t k);
		void   DeleteHitBoxAt(std::vector<HitBox>::const_iterator it);
		size_t GetLinesAmount();
		size_t GetHitBoxAmount();
	public:
		void					   CheckCollision(PhysicObject2D* obj);
		bool					   CheckLineCollision(Line line);
		std::vector<Physics::Line> GetLines(HitBox hb);
	private:
		std::string dataPath = "";
		float		lineColor[3];
		float		hbColor  [3];
		float		deltaCollision;
		bool		objVisability;
	private:
		std::vector<Line>	lines;
		std::vector<HitBox> hitboxes;
	};
}