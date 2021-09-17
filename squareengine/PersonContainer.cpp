#include "PersonContainer.h"
#include "EngineFunctions.hpp"

PersonContainer::PersonContainer(std::string dataPath)
{
#if IS_ENGINE_MODE
	applog.AddLog(OBJECT_LOG, "Инициализация\n");
#endif // IS_ENGINE_MODE

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

			/************************/

			/* Получение позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");

			DirectX::XMFLOAT2 position = { pos_x, pos_y };

			/******************/

			/* Получение пути к изображению */

			std::string pathToSprite = obj.at("path");

			/***************************/

			/* Инициализация модели */

			persons.emplace_back(std::make_unique<Person>(name, position, pathToSprite));

#if IS_ENGINE_MODE
			std::ostringstream oss;
			oss << "Добавлен персонаж: " << name << "\n";

			applog.AddLog(OBJECT_LOG, oss.str());
#endif // IS_ENGINE_MODE
			/**************************************************/
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

void PersonContainer::DrawLog()
{
	ImGui::Begin("Лог", NULL, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);

	applog.Draw("Лог", NULL);
	ImGui::End();
}

std::vector<std::unique_ptr<Person>>* PersonContainer::GetPersons()
{
	return &persons;
}
