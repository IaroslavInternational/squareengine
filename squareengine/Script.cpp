#include "Script.h"

void Script::AddCommand(std::pair<std::string, int> cmd)
{
	commands.push_back(cmd);
}

void Script::NextCommand()
{
	if (active_cmd != commands.size())
	{
		active_cmd++;
	}
	else
	{
		active_cmd = 0;
	}
}
