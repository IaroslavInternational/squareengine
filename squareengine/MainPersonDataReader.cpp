#include "MainPersonDataReader.h"

MainPersonDataReader::MainPersonDataReader(std::string dataPath)
	:
	dataPath(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными об о персонажах");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Получение имени объекта */

			name = obj.at("name");
			std::transform(name.begin(), name.end(), name.begin(), tolower);

			/***************************/

			/* Получение позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");

			position = { pos_x, pos_y };

			/*********************/

			/* Получение пути к изображению */

			pathToSprite = obj.at("path");

			/********************************/

			/* Получение настроек хит-бокса */

			hb_coord.x = obj.at("hb-ltx");
			hb_coord.y = obj.at("hb-lty");
			hb_coord.z = obj.at("hb-rbx");
			hb_coord.w = obj.at("hb-rby");

			/********************************/

			/* Получение настроек эффекта */

			eff_d = obj.at("eff-d");
			eff_t = obj.at("eff-t");
			eff_a = obj.at("eff-a");

			/******************************/

			/* Получение настройки скорости */

			speed = obj.at("speed");

			/********************************/

			/* Получение настройки слоя */

			layer = obj.at("layer");

			/****************************/
		}
	}
}
