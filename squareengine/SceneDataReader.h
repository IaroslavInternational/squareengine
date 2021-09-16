#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(std::string path);
	~SceneDataReader();
public:
	std::string GetPersonContainerPath() const;		// Путь к файлу с данными о контейнере объектов
private:
	// 0 - Путь к персонажам
	std::vector<std::string>& GetPaths();
private:
	std::vector<std::string> paths;
};

