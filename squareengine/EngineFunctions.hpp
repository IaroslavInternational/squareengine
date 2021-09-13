#pragma once

#include "EngineConverter.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif // IS_ENGINE_MODE

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

// Специальные функции для движка
namespace EngineFunctions
{
#if IS_ENGINE_MODE
	/* 
	Установить новое значение для параметра в файле .json
	objectName - имя изменяемого объекта
	param - параметр для замены
	path - путь к файлу
	val - значение
	Applog - указатель на лог
	*/
	template<typename T>
	void static SetNewValue(std::string objectName, std::string param, T val, std::string path, AppLog* applog)
#else
	template<typename T>
	void static SetNewValue(std::string objectName, std::string param, T val, std::string path)
#endif // IS_ENGINE_MODE
	{
		// Открытие файла
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("Не удаётся открыть файл с данными");
		}

#if IS_ENGINE_MODE
		std::ostringstream ostrlog;
		ostrlog << "Установка [" << param << " : " << std::to_string(val) << "] " << "для [" << objectName << "]\n";

		applog->AddLog(SYSTEM_LOG, ostrlog.str().c_str());
#endif // IS_ENGINE_MODE

		// Чтение файла
		json j;
		dataFile >> j;

		// Закрытие файла
		dataFile.close();

		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			for (auto& obj : j.at(objectName))
			{
				obj[param] = val;
			}
		}

		// Запись в файл
		std::ofstream ostr(path);
		ostr << j.dump();

		// Закрытие файла
		ostr.close();
	}

	// Соеденить два объекта в одну строку через пробел
	template <typename T>
	std::string static AttachStrings(T str1, T str2) 
	{
		std::ostringstream oss;
		oss << str1 << " " << str2;

		return oss.str();
	}

	// Удаление объекта из файла json
	void static DeleteJsonObject(std::string objectName, std::string path)
	{
		// Открытие файла
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("Не удаётся открыть файл с данными");
		}

		// Чтение файла
		json j;
		dataFile >> j;

		// Закрытие файла
		dataFile.close();

		j[objectName] = "";

		auto j_str = j.dump();

		auto pos = j_str.find(objectName);

		for (auto i = pos - 1; i < objectName.length() + 7; i++)
		{
			j_str[i] = '*';
		}

		j_str.erase(std::remove(j_str.begin(), j_str.end(), '*'), j_str.end());

		// Запись в файл
		std::ofstream ostr(path);
		ostr << j_str;

		// Закрытие файла
		ostr.close();
	}
}