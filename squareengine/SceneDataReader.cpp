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
			/* ������ ���� ������� */
			
			paths.emplace_back(obj.at("objectsPath"));

			/***********************/

			/* ������ ���� �������� ����� */

			paths.emplace_back(obj.at("mainPersonPath"));

			/*************************/

			/* ������ ���� ��������� */

			//paths.emplace_back(obj.at("pLightsPath"));

			/*************************/

			/* ������ ���� ����� */

			//paths.emplace_back(obj.at("camerasPath"));

			/*************************/
		}
	}
}

SceneDataReader::~SceneDataReader()
{
}

std::string SceneDataReader::GetPersonContainerPath() const
{
	return paths[0];
}

std::string SceneDataReader::GetMainPersonDataPath() const
{
	return paths[1];
}

std::vector<std::string>& SceneDataReader::GetPaths()
{
	return paths;
}
