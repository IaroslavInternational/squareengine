#pragma once

#include "TriggerType.hpp"
#include "HitBox.h"
#include "Line.h"
#include <vector>

class Graphics;

class Trigger
{
public:
	Trigger(std::string name, float start_x, float start_y, float end_x, float end_y,       TriggerType type, std::string goal);
	Trigger(std::string name, const DirectX::XMFLOAT2& start, const DirectX::XMFLOAT2& end, TriggerType type, std::string goal);
	Trigger(std::string name, const Physics::Line& line,								    TriggerType type, std::string goal);
public:
	void Translate(const DirectX::XMFLOAT2& delta);
	void Draw(Graphics& gfx);
	bool IsCollide(HitBox hitbox);
	bool IsCollide(const Physics::Line& line);
public:
	TriggerType GetType() const;
	std::string GetName() const;
	std::string GetGoal() const;
private:
	std::vector<Physics::Line> GetLines(HitBox hitbox);
private:
	std::string name;
	TriggerType type = TriggerType::SceneTrigger;
private:
	std::string   goal;
	Physics::Line line;
};