#include "InteractableObject2DContainer.h"

#include "EngineFunctions.hpp"

InteractableObject2DContainer::InteractableObject2DContainer(std::string dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� �� � ��������");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* ��������� ����� ������� */

			std::string name = m.key();
			std::transform(name.begin(), name.end(), name.begin(), tolower);

			/***************************/

			/* ��������� ������� */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");

			DirectX::XMFLOAT2 position = { pos_x, pos_y };

			/*********************/

			/* ��������� ���� � ����������� */

			std::string pathToSprite = obj.at("path");

			/********************************/

			/* ������������� ������� */

			objects.emplace_back(InteractableObject2D(name, position, pathToSprite));

			/*************************/
		}
	}

	objects.shrink_to_fit();
}

void InteractableObject2DContainer::Translate(DirectX::XMFLOAT2 delta)
{
	for (auto& obj : objects)
	{
		obj.Translate(delta);
	}
}
