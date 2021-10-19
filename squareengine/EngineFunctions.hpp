#pragma once

#include "HitBox.h"
#include "AppLog.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

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

	// ��������� ��� ������� � ���� ������ ����� ������
	template <typename T>
	inline std::string static AttachStrings(T str1, T str2)
	{
		std::ostringstream oss;
		oss << str1 << " " << str2;

		return oss.str();
	}

	// �������� ������� �� ����� json
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
}