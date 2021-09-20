#pragma once

#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "AdapterData.h"
#include "AppLog.h"

#include <map>
#include <string>

#include "PersonContainer.h"

class GUISystem
{
public:
	GUISystem(std::shared_ptr<Window> wnd, std::vector<std::unique_ptr<Person>>* pv_ptr);
public: // ������� ������
	// �������� ���������
	void Show();

	// ������ ���������
	void Hide();

	void AddLog(const char* text);
private: // ������ ��������� � ��������� �������
	void SetGUIColors();

	// ��������� ��������� � �������� ������
	void SetPanelWidthAndPosition(int corner, float width, float height, float x_offset = 0.0f, float y_offset = 0.0f);

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

	// ��������� ������� ������
	void DisableSides();
private: // ������ ��������� ���������� �����������
	// �������� ������ � ����������� �� FPS � ����������� �������� 
	void ShowFPSAndGPU();

	void ShowMouseCoordinates();

	void ShowLog();

	void ShowPersonList();
	void ShowPersonControl();
private: // ���������� ��������� �������
	// ����� ������ �������� �� �����
	bool ShowPersonEnum = true;

	// ������ ������ ���������� ����������� ��������
	bool ShowPersonSettings = true;

	// FPS � ���������� � ����������� ��������
	bool ShowHardwareInfo = true;

	// ���
	bool ShowLogs = true;

	// ����� ������ ��������� �� �����
	bool ShowTriggersList = false;

	// ������ ������ ��������� �� �����
	bool ShowTriggersSettings = false;
private:
	std::shared_ptr<Window> wnd;

	// ���
	AppLog applog;
	std::map<std::wstring, double> gpu_desc;
	std::pair<bool, std::pair<int, int>> pointSet = { false, {0, 0} };
	bool IsDrawingHitBox = false;
private:
	std::string personSelected = "";
	std::vector<std::unique_ptr<Person>>* pv_ptr;
};

