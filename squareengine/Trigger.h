#pragma once

#include "TriggerType.hpp"
#include "HitBox.h"
#include "Line.h"
#include <vector>

class Trigger
{
public:
	Trigger(std::string name, float start_x, float start_y, float end_x, float end_y);
	Trigger(std::string name, const DirectX::XMFLOAT2& start, const DirectX::XMFLOAT2& end);
	Trigger(std::string name, const Physics::Line& line);
public:
	bool IsCollide(HitBox& hitbox);
	bool IsCollide(const Physics::Line& line);
public:
	TriggerType GetType() const;
	std::string GetName() const;
private:
	std::vector<Physics::Line> GetLines(HitBox& hitbox);
private:
	std::string name;
	TriggerType type = TriggerType::SceneTrigger;
private:
	std::string goal = "Scene 2";
	Physics::Line line;
};