#include "MainPersonDataReader.h"

#include "EngineFunctions.hpp"

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
			size_t key_r = obj.at("chr-r");
			size_t key_g = obj.at("chr-g");
			size_t key_b = obj.at("chr-b");

			key = Color(key_r, key_g, key_b);

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
			
			/* ��������� ��������� ������ ������ */

			camMode = obj.at("camera-mode");

			/*************************************/

			/* ��������� ��������� �������� */

			anim_ps = obj.at("a-ps");
			anim_pe = obj.at("a-pe");
			anim_fw = obj.at("a-fw");
			anim_fh = obj.at("a-fh");
			anim_fa = obj.at("a-fa");
			anim_ft = obj.at("a-ft");

			/********************************/
		}
	}
}
