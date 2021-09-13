#pragma once

#include "Window.h"

#if IS_ENGINE_MODE
#include "ImguiManager.h"
#endif

#include "EngineTimer.h"

//#include "SceneObjects.h"

#include "Character.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene(std::string sceneName,	   std::shared_ptr<Window> _wnd, 
		  std::string data);
	~Scene();

	/***** /������ �����\ *****/

	// ��������� ������� ������
	void ProcessInput(float dt);

	// ��������� �����
	void Render(float dt);

	std::pair<std::string, bool> IsOnTheSceneTrigger();

	// ���������� ������ �� �������� ���������
	void ResetPos();

	// ������ ����� �����
	std::string GetName() const;

	/***** \������ �����/ *****/
private:
#if IS_ENGINE_MODE
	/* -- ��������� -- */

	/**** ������ ****/

	// ��������� ��������� � �������� ������
	void SetPanelWidthAndPos(int corner, float width, float height, float x_offset = 0.0f, float y_offset = 0.0f);

	// ������ ������ ����
	void ShowMenu();

	// ����� ������
	void ShowLeftSide();

	// ������ ������
	void ShowRightSide();

	// ����� ������ ������
	void ShowLeftBottomSide();

	// ������ ������
	void ShowBottomPanel();	

	/****************/

	// ������������ ����������� ��������
	void ShowTrigCheck();

	// �������� FPS � ����������� �������� *����� ������ ������*
	void ShowFPSAndGPU();

	// �������� ��� *������ ������*
	void ShowLog();

	// �������� �������� ����� *��������� ����*
	void ShowNodeEditor();

	// ��������� ������� ������
	void DisableSides();

	// ��������� ��� ������
	void DisableAll();

	// �������� ���������
	void ShowGui();

	// �������� ����� ImGui
	void SetGuiColors();

	/*******************/
	
	// ����-��������� 
	void ShowImguiDemoWindow();

	/*******************/

#endif // IS_ENGINE_MODE
private:
	// �������������� �����
	std::string sceneName;

	// ���������
#if IS_ENGINE_MODE
	bool savingDepth = false;
#endif // IS_ENGINE_MODE

	bool showDemoWindow = true;
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;

#if IS_ENGINE_MODE
	/* ��������� */

	// ����� ������ ������� �� �����
	bool ShowModelsList = true;

	// ������ ������ ���������� ����������� �������
	bool ShowModelsSettings = true;

	// FPS � ���������� � ����������� ��������
	bool ShowHardwareInfo = true;

	// ���
	bool ShowLogs = true;

	// ����� ������ ��������� �� �����
	bool ShowTriggersList = false;

	// ������ ������ ��������� �� �����
	bool ShowTriggersSettings = false;

	// ����� ������ �������� ���������� ����� �� �����
	bool ShowPLightsList = false;

	// ������ ������ ���������� ����������� �������� ���������� �����
	bool ShowPLightsSettings = false;

	// ����� ������ ����� �� ����� � �����������
	bool ShowCamsList = false;

	// �������� �����
	bool ShowNodeEditorWnd = false;

	/*************/
#endif // IS_ENGINE_MODE
private:
	// ��������� �� ������� ���� 
	std::shared_ptr<Window> wnd;

	// ������ ����
	/********* -�������- *********/

	//SceneObjects objects;
	Character person;

	/*****************************/
};