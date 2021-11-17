#pragma once

#include "Script.h"

class ScriptCompiler
{
public:
	ScriptCompiler(const std::string& filename);
public:
	Script GetScript();
private:
	std::string					FindNextLine(std::string& script);
	std::pair<std::string, int> SplitCommand(std::string line);
private:
	Script script;
};

