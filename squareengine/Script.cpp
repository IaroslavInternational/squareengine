#include "Script.h"

void Script::SetGoal(int goal)
{
	if (!IsSetGoal)
	{
		this->goal = goal;
		IsSetGoal = true;
	}
}

void Script::AddCommand(std::pair<std::string, int> cmd)
{
	commands.push_back(cmd);
}

int Script::GetGoal()
{
	return goal;
}

std::pair<std::string, int> Script::GetCommand()
{
	return commands[active_cmd];
}

void Script::NextCommand()
{
	IsSetGoal = false;

	if (active_cmd != commands.size() - 1)
	{
		active_cmd++;
	}
	else
	{
		active_cmd = 0;
	}
}
