#pragma once

#include "Window.h"

#include "EngineTimer.h"
#include "SceneDataReader.h"
#include "PersonContainer.h"

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
	bool showDemoWindow = true;
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;

#if IS_ENGINE_MODE
	/* ��������� */

	// �������� �����
	bool ShowNodeEditorWnd = false;

	/*************/
#endif // IS_ENGINE_MODE
private:
	// ��������� �� ������� ���� 
	std::shared_ptr<Window> wnd;

	GUISystem gui;

	SceneDataReader sdr;

	/********* -�������- *********/

	PersonContainer pc;

	/*****************************/
};