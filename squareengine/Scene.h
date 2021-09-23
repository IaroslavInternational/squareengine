#pragma once

#include "Window.h"

#include "EngineTimer.h"
#include "SceneDataReader.h"
#include "PersonContainer.h"
#include "MainPerson.h"

#include "Font.h"
#include "GUISystem.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene(std::string name,	std::shared_ptr<Window> _wnd, 
		  std::string data);
	~Scene();

	/***** /������ �����\ *****/

	// ��������� ������� ������
	void ProcessInput(float dt);

	// ��������� �����
	void Render(float dt);

	std::pair<std::string, bool> IsOnTheSceneTrigger();

	// ������ ����� �����
	std::string GetName() const;

	/***** \������ �����/ *****/
private:
	// �������������� �����
	std::string name;

	// ���������
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;
private:
	// ��������� �� ������� ���� 
	std::shared_ptr<Window> wnd;

	// ���������
	GUISystem gui;

	// ����� ������ �����
	SceneDataReader sdr;
	MainPersonDataReader mdr;

	/********* -�������- *********/

	PersonContainer pc;
	MainPerson hero;
	/*****************************/
};