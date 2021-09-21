#include "PersonContainer.h"
#include "EngineFunctions.hpp"

PersonContainer::PersonContainer(std::string dataPath)
	:
	dataPath(dataPath)
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

			std::string name = obj.at("name");
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

			/* ��������� �������� ���-����� */

			DirectX::XMINT4 hb_coord;
			hb_coord.x = obj.at("hb-ltx");
			hb_coord.y = obj.at("hb-lty");	
			hb_coord.z = obj.at("hb-rbx");
			hb_coord.w = obj.at("hb-rby");

			/********************************/
			
			/* ��������� �������� ������� */

			float eff_d = obj.at("eff-d");
			float eff_t = obj.at("eff-t");
			float eff_a = obj.at("eff-a");
			
			/******************************/

			/* ��������� ��������� �������� */
			
			float speed = obj.at("speed");
			
			/********************************/

			/* ������������� ������� */

			persons.emplace_back(std::make_unique<Person>(name, position, pathToSprite, HitBox(hb_coord), speed, eff_d, eff_t, eff_a));

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
