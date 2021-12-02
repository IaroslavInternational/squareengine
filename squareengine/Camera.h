#pragma once

#include "PhysicsEngine.h"
#include <memory>

class Map;
class MainPerson;

class Camera
{
public:
	friend class GUISystem;
public:
	Camera(Map* map, MainPerson* hero, std::shared_ptr<Physics::PhysicsEngine> phEngPtr, std::string dataPath);
public:
	void			  Init();
	void			  Translate(DirectX::XMFLOAT2 delta);
	void			  TranslateAll(DirectX::XMFLOAT2 delta);
	void			  ToggleNoClip();
	void			  SetPosition(DirectX::XMFLOAT2 pos);
	DirectX::XMFLOAT2 GetPosition();
	void			  SetInitPosition(DirectX::XMFLOAT2 pos);
private:
	Map*									map;
	MainPerson*								hero;
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;
private:
	std::string		  dataPath;
	bool			  noclip;
	float			  noclipSpeed;
	DirectX::XMFLOAT2 initPosition;
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
};
