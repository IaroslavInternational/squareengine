#pragma once

#include "TriggerType.hpp"
#include "HitBox.h"
#include "Line.h"
#include <vector>

class Trigger
{
public:
	Trigger(float start_x, float start_y, float end_x, float end_y);
	Trigger(const DirectX::XMFLOAT2& start, const DirectX::XMFLOAT2& end);
	Trigger(const Physics::Line& line);
public:
	bool IsCollide(HitBox& hitbox);
	bool IsCollide(const Physics::Line& line);
private:
	std::vector<Physics::Line> GetLines(HitBox& hitbox);
private:
	TriggerType type = TriggerType::SceneTrigger;
private:
	std::string goal = "Scene 2";
	Physics::Line line;
};