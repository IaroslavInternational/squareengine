#pragma once

#include <string>
#include <vector>

class Script
{
public:
	Script() = default;
public:
	void AddCommand(std::pair<std::string, int> cmd);
public:
	void NextCommand();
private:
	size_t active_cmd = 0;
private:
	std::vector<std::pair<std::string, int>> commands;
};

