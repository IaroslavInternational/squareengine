#pragma once

#include "Trigger.h"
#include <map>

class TriggerContainer
{
public:
	TriggerContainer();
public:
	std::pair<TriggerType, bool> Check(const HitBox& hitbox);
private:
	std::vector<Trigger> triggers;
};

