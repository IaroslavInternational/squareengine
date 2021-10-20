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
		throw ("�� ������ ������� ���� � ������� � �����");
	}

	json j;
	dataFile >> j;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* ������ ���� � ������ ���������� */
			
			paths.emplace_back(obj.at("objectsPath"));

			/***********************/

			/* ������ ���� � ������ �������� ����� */

			paths.emplace_back(obj.at("mainPersonPath"));

			/******************************/	
			
			/* ������ ���� ������ � ������ � ������ */

			paths.emplace_back(obj.at("physicsPath"));

			/*******************************/

			/* ������ ���� � ������ ������������� �������� */
			
			paths.emplace_back(obj.at("interactableObjectsPath"));

			/***********************/	
			
			/* ������ ���� � ������ ������ */
			
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
