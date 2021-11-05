#pragma once

#include "HitBox.h"
#include "AnimationData.h"
#include "AppLog.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "dirent.h"

using json = nlohmann::json;
using namespace std::string_literals;

// ����������� ������� ��� ������
namespace EngineFunctions
{
	/* 
	���������� ����� �������� ��� ��������� � ����� .json
	objectName - ��� ����������� �������
	param - �������� ��� ������
	path - ���� � �����
	val - ��������
	Applog - ��������� �� ���
	*/
	template<typename T>
	inline void static SetNewValue(std::string objectName, std::string param, T val, std::string path, AppLog* applog)
	{
		// �������� �����
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("�� ������ ������� ���� � �������");
		}

		std::ostringstream ostrlog;
		ostrlog << "��������� [" << param << " : " << val << "] " << "��� [" << objectName << "]\n";

		applog->AddLog(SYSTEM_LOG, ostrlog.str().c_str());

		// ������ �����
		json j;
		dataFile >> j;

		// �������� �����
		dataFile.close();

		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			for (auto& obj : j.at(objectName))
			{
				obj[param] = val;
			}
		}

		// ������ � ����
		std::ofstream ostr(path);
		ostr << j.dump();

		// �������� �����
		ostr.close();
	}

	// ��������� ������ � hitbox
	inline void static SaveHitBoxData(std::string objectName, HitBox hitbox, std::string path, AppLog* applog)
	{
		EngineFunctions::SetNewValue<float>(
			objectName,
			"hb-ltx", hitbox.GetCoordinates().x,
			path,
			applog
			);

		EngineFunctions::SetNewValue<float>(
			objectName,
			"hb-lty", hitbox.GetCoordinates().y,
			path,
			applog
			);

		EngineFunctions::SetNewValue<float>(
			objectName,
			"hb-rbx", hitbox.GetCoordinates().z,
			path,
			applog
			);

		EngineFunctions::SetNewValue<float>(
			objectName,
			"hb-rby", hitbox.GetCoordinates().w,
			path,
			applog
			);
	}

	// ��������� ������ �� ��������
	inline void static SaveAnimationData(std::string objectName, AnimationData data, std::string path, AppLog* applog)
	{
		EngineFunctions::SetNewValue<int>(
			objectName,
			"a-ps", data.pStart,
			path,
			applog
			);

		EngineFunctions::SetNewValue<int>(
			objectName,
			"a-pe", data.pStart,
			path,
			applog
			);

		EngineFunctions::SetNewValue<int>(
			objectName,
			"a-fw", data.width,
			path,
			applog
			);

		EngineFunctions::SetNewValue<int>(
			objectName,
			"a-fh", data.height,
			path,
			applog
			);

		EngineFunctions::SetNewValue<float>(
			objectName,
			"a-ft", data.ft,
			path,
			applog
			);

		EngineFunctions::SetNewValue<int>(
			objectName,
			"a-fa", data.frames,
			path,
			applog
			);
	}

	// �������� ��������� � ������
	inline std::string static StrReplace(const std::string& inputStr, const std::string& src, const std::string& dst)
	{
		std::string result(inputStr);

		size_t pos = result.find(src);

		while (pos != std::string::npos)
		{
			result.replace(pos, src.size(), dst);
			pos = result.find(src, pos);
		}

		return result;
	}

	// ������� ������ �� ����� json
	inline void static DeleteJsonObject(std::string objectName, std::string path)
	{
		// �������� �����
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("�� ������ ������� ���� � �������");
		}

		// ������ �����
		json j;
		dataFile >> j;

		// �������� �����
		dataFile.close();

		j[objectName] = "";

		auto j_str = j.dump();

		auto pos = j_str.find(objectName);
		
		if (j_str[pos + objectName.length() + 4] == '}')
		{
			for (auto i = pos - 2; i < pos + objectName.length() + 4; i++)
			{
				j_str[i] = '*';
			}
		}
		else
		{
			for (auto i = pos - 1; i < pos + objectName.length() + 5; i++)
			{
				j_str[i] = '*';
			}
		}

		j_str.erase(std::remove(j_str.begin(), j_str.end(), '*'), j_str.end());

		// ������ � ����
		std::ofstream ostr(path);
		ostr << j_str;

		// �������� �����
		ostr.close();
	}

	// �������� ��� �������
	inline const static std::string& GetProjectName()
	{
		std::ifstream dataFile("current_project_setup.json");
		if (!dataFile.is_open())
		{
			throw ("�� ������ ������� ���� � ������� � �������");
		}

		json j;
		dataFile >> j;

		dataFile.close();

		static std::string name = "";

		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			auto d = m.key();

			for (const auto& obj : j.at(d))
			{
				/* ��������� ����� ������� */

				name = obj.at("name");
				
				break;

				/***************************/
			}
		}

		return name;
	}

	// �������� ������ ��������� ����
	inline std::vector<std::string> static GetScenesNames()
	{
		std::vector<std::string> names;

		int file_count = 0;
		int dir_count = 0;
		int total = 0;

		DIR* dirp;
		struct dirent* entry;

		std::ostringstream path;
		path << "Projects\\" << GetProjectName() << "\\Scenes\\";

		dirp = opendir(path.str().c_str());
		while ((entry = readdir(dirp)) != NULL) 
		{
			total++;
			if (std::string(entry->d_name).find('.') != std::string(entry->d_name).npos)
			{
				file_count++;
			}
		}
		dir_count = total - file_count;
		closedir(dirp);

		for (size_t i = 0; i < dir_count; i++)
		{
			std::ostringstream oss;
			oss << "Scene " << i + 1;

			names.push_back(oss.str());
		}

		return names;
	}

	// �������� ��� �������
	inline void static ChangeObjectName(const std::string& objectSrc, const std::string& objectDst, const std::string& path)
	{
		// �������� �����
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("�� ������ ������� ���� � �������");
		}

		// ������ �����
		json j;
		dataFile >> j;

		// �������� �����
		dataFile.close();

		auto str = j.dump();
		str = EngineFunctions::StrReplace(str, objectSrc, objectDst);

		// ������ � ����
		std::ofstream ostr(path);
		ostr << str;

		// �������� �����
		ostr.close();
	}
}