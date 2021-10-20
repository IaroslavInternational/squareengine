#include "SceneDataReader.h"

#include <sstream>
#include <fstream>
#include <filesystem>

#include "json.hpp"

SceneDataReader::SceneDataReader(std::string path)
{
	using json = nlohmann::json;
	using namespace std::string_literals;

	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о сцене");
	}

	json j;
	dataFile >> j;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Запись пути к даннам персонажей */
			
			paths.emplace_back(obj.at("objectsPath"));

			/***********************/

			/* Запись пути к даннам главного героя */

			paths.emplace_back(obj.at("mainPersonPath"));

			/******************************/	
			
			/* Запись пути данных к даннам о физике */

			paths.emplace_back(obj.at("physicsPath"));

			/*******************************/

			/* Запись пути к даннам интерактивных объектов */
			
			paths.emplace_back(obj.at("interactableObjectsPath"));

			/***********************/	
			
			/* Запись пути к даннам камеры */
			
			paths.emplace_back(obj.at("cameraPath"));

			/***********************/
		}
	}
}

std::string SceneDataReader::GetPersonContainerPath() const
{
	return paths[0];
}

std::string SceneDataReader::GetMainPersonDataPath() const
{
	return paths[1];
}

std::string SceneDataReader::GetPhysicsDataPath() const
{
	return paths[2];
}

std::string SceneDataReader::GetInteractableObjectsDataPath() const
{
	return paths[3];
}

std::string SceneDataReader::GetCameraDataPath() const
{
	return paths[4];
}

std::vector<std::string>& SceneDataReader::GetPaths()
{
	return paths;
}
