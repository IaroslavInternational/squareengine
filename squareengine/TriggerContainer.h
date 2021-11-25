#pragma once

#include "Trigger.h"
#include "ContainerBase.hpp"

#include <map>
#include <optional>

class TriggerContainer : public ContainerBase<Trigger>
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
};

