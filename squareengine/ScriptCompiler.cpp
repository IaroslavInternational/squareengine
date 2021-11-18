#include "ScriptCompiler.h"

#include "EngineFunctions.hpp"

ScriptCompiler::ScriptCompiler(const std::string& filename)
{
	// Открытие файла
	std::ifstream dataFile(filename);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть скрипт");
	}

	std::string script_str = "";
	std::getline(dataFile, script_str, '\0');

	// Закрытие файла
	dataFile.close();

	std::vector<std::string> lines;

	script_str = EngineFunctions::StrReplace(script_str, "\n", "");

	while (script_str.find(';') != script_str.npos)
	{
		lines.push_back(FindNextLine(script_str));
	}

	for (auto& l : lines)
	{
		script.AddCommand(SplitCommand(l));
	}
}

Script ScriptCompiler::GetScript()
{
	return script;
}

std::string ScriptCompiler::FindNextLine(std::string& script)
{
	auto terminal_pos = script.find(';');
	
	std::string line(script.c_str(), script.c_str() + terminal_pos);
	script.erase(0, terminal_pos + 1);

	return line;
}

std::pair<std::string, int> ScriptCompiler::SplitCommand(std::string line)
{
	auto terminal_pos = line.find('(');
	std::string cmd(line.c_str(), line.c_str() + terminal_pos);

	auto terminal_pos_end = line.find(')');
	std::string val(line.c_str() +  terminal_pos + 1, line.c_str() + terminal_pos_end);

	return std::pair<std::string, int>(cmd, std::stoi(val));
}
