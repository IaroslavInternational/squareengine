#include "TriggerContainer.h"

TriggerContainer::TriggerContainer(std::string dataPath)
	:
	dataPath(dataPath)
{
}

std::optional<std::pair<TriggerType, std::string>> TriggerContainer::Check(HitBox& hitbox)
{
	for (auto& t : triggers)
	{
		if (t.IsCollide(hitbox))
		{
			return std::pair<TriggerType, std::string>(t.GetType(), t.GetName());
		}
	}
}
