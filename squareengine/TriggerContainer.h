#pragma once

#include "Trigger.h"
#include <map>
#include <optional>

class TriggerContainer
{
public:
	TriggerContainer(std::string dataPath);
public:
	void											   Draw(Graphics& gfx);
	std::optional<std::pair<TriggerType, std::string>> Check(HitBox hitbox);
public:
	const Trigger& GetTriggerByName(std::string name);
private:
	std::string dataPath;
private:
	std::vector<Trigger> triggers;
};

