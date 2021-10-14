#include "MainPersonDataReader.h"

MainPersonDataReader::MainPersonDataReader(std::string dataPath)
	:
	dataPath(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� �� � ����������");
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

			name = obj.at("name");
			std::transform(name.begin(), name.end(), name.begin(), tolower);

			/***************************/

			/* ��������� ������� */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");

			position = { pos_x, pos_y };

			/*********************/

			/* ��������� ���� � ����������� */

			pathToSprite = obj.at("path");

			/********************************/

			/* ��������� �������� ���-����� */

			hb_coord.x = obj.at("hb-ltx");
			hb_coord.y = obj.at("hb-lty");
			hb_coord.z = obj.at("hb-rbx");
			hb_coord.w = obj.at("hb-rby");

			/********************************/

			/* ��������� �������� ������� */

			eff_d = obj.at("eff-d");
			eff_t = obj.at("eff-t");
			eff_a = obj.at("eff-a");

			/******************************/

			/* ��������� ��������� �������� */

			speed = obj.at("speed");

			/********************************/

			/* ��������� ��������� ���� */

			layer = obj.at("layer");

			/****************************/
		}
	}
}
