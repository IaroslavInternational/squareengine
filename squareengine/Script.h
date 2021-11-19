#pragma once

#include <string>
#include <vector>

class Script
{
public:
	Script() = default;
public:
	bool IsEmpty();
	void ForceGoalTranslate(float delta);
	void SetGoal(int goal);
	void AddCommand(std::pair<std::string, int> cmd);
public:
	int							GetGoal();
	std::pair<std::string, int> GetCommand();
	void						NextCommand();
private:
	size_t active_cmd = 0;
	int	   goal		  = 0;
	bool   IsSetGoal  = false;
private:
	std::vector<std::pair<std::string, int>> commands;
};

