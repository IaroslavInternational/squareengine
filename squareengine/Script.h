#pragma once

#include <string>
#include <vector>
#include <map>

class Script
{
public:
	Script();
public:
	void AddCommand(std::string, int);
private:
	std::vector<std::map<std::string, int>> cmd;
};

