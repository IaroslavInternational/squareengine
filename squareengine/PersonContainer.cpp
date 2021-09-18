#include "PersonContainer.h"
#include "EngineFunctions.hpp"

PersonContainer::PersonContainer(std::string dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными об о объектах");
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

			std::string name = obj.at("name");
			std::transform(name.begin(), name.end(), name.begin(), tolower);

			/***************************/

			/* Получение позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");

			DirectX::XMFLOAT2 position = { pos_x, pos_y };

			/*********************/

			/* Получение пути к изображению */

			std::string pathToSprite = obj.at("path");

			/********************************/

			/* Получение настроек эффекта */

			float eff_d = obj.at("eff-d");
			float eff_t = obj.at("eff-t");
			float eff_a = obj.at("eff-a");
			
			/******************************/

			/* Инициализация объекта */

			persons.emplace_back(std::make_unique<Person>(name, position, pathToSprite, eff_d, eff_t, eff_a));

			/*************************/
		}
	}

	persons.shrink_to_fit();
}

void PersonContainer::Draw(Graphics& gfx)
{
	for (auto& p : persons)
	{
		p->Draw(gfx);
	}
}

std::vector<std::unique_ptr<Person>>* PersonContainer::GetPersons()
{
	return &persons;
}
