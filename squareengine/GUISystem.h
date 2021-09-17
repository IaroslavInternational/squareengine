#pragma once

#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "AdapterData.h"

#include <map>
#include <string>

#include "PersonContainer.h"

class GUISystem
{
public:
	GUISystem(std::vector<std::unique_ptr<Person>>* pv_ptr);
public: // ������� ������
	// �������� ���������
	void Show();

	// ������ ���������
	void Hide();
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
	std::map<std::wstring, double> gpu_desc;
private:
	std::string personSelected = "";
	std::vector<std::unique_ptr<Person>>* pv_ptr;
};

