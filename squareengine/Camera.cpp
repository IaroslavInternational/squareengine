#include "Camera.h"

#include "Map.h"

#include "EngineFunctions.hpp"

Camera::Camera(Map* map, MainPerson* hero, std::shared_ptr<Physics::PhysicsEngine> phEngPtr, std::string dataPath)
	:
	hero(hero),
	map(map),
	phEngPtr(phEngPtr),
	dataPath(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о камере");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Получение начальной позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");

			initPosition = { pos_x, pos_y };

			/*******************************/

			/* Получение настроек no-clip */

			noclip = obj.at("nc-able");
			noclipSpeed = obj.at("nc-speed");

			/********************************/
		}
	}
}

void Camera::Init()
{
	SetPosition(initPosition);
}

void Camera::Translate(DirectX::XMFLOAT2 delta)
{
	position.x -= delta.x;
	position.y -= delta.y;

	phEngPtr->TranslateObjects(delta);
	map->Translate(delta);
}

void Camera::TranslateAll(DirectX::XMFLOAT2 delta)
{
	delta.x *= noclipSpeed;
	delta.y *= noclipSpeed;

	Translate(delta);
	hero->Translate(delta);
}

bool Camera::GetNoClipState()
{
	return noclip;
}

void Camera::SetPosition(DirectX::XMFLOAT2 pos)
{
	DirectX::XMFLOAT2 dif;
	dif.x = position.x - pos.x;
	dif.y = position.y - pos.y;

	position = pos;

	phEngPtr->TranslateObjects(dif);
	map->Translate(dif);
	hero->Translate(dif);
}

DirectX::XMFLOAT2 Camera::GetPosition()
{
	return position;
}

void Camera::SetInitPosition(DirectX::XMFLOAT2 pos)
{
	initPosition = pos;
}
