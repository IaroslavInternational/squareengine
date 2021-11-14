#pragma once

#include "Trigger.h"
#include <map>
#include <optional>

class TriggerContainer
{
public:
	friend class GUISystem;
public:
	TriggerContainer(std::string dataPath);
public:
	void				   Draw(Graphics& gfx);
	void				   Translate(const DirectX::XMFLOAT2& delta);
	void				   UpdateLineAt(size_t k, Physics::Line line);
	std::optional<Trigger> Check(HitBox hitbox);
private:
	std::string dataPath;
private:
	std::vector<Trigger> triggers;
};

