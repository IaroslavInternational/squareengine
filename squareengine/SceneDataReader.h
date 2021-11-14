#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(std::string path);
public:
	std::string GetPersonContainerPath() const;				// Путь к файлу с данными о контейнере объектов
	std::string GetMainPersonDataPath() const;				// Путь к файлу с данными о главном персонаже
	std::string GetPhysicsDataPath() const;					// Путь к файлу с данными о физике сцены
	std::string GetInteractableObjectsDataPath() const;		// Путь к файлу с данными об интерактивных объектах
	std::string GetCameraDataPath() const;					// Путь к файлу с данными о камере
	std::string GetTriggerContainerDataPath() const;		// Путь к файлу с данными о триггерах
private:
	// 0 - Путь к персонажам
	// 1 - Путь к главному герою
	// 2 - Путь к физике
	// 3 - Путь к интерактивным объектам
	// 4 - Путь к камере
	// 5 - Путь к триггерам
	std::vector<std::string>& GetPaths();	
private:
	std::vector<std::string> paths;
};

