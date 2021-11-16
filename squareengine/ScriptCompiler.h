#pragma once

#include "Script.h"

class ScriptCompiler
{
public:
	ScriptCompiler(const char* filename);
	ScriptCompiler(const std::string& filename);
private:
	Script script;
};

