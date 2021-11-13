#pragma once

#include "Trigger.h"
#include <map>
#include <optional>

class TriggerContainer
{
public:
	TriggerContainer(std::string dataPath);
public:
	std::optional<std::pair<TriggerType, std::string>> Check(HitBox& hitbox);
private:
	std::string dataPath;
private:
	std::vector<Trigger> triggers;
};

