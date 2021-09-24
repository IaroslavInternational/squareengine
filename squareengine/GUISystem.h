#pragma once

#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "AdapterData.h"
#include "AppLog.h"
#include "PersonContainer.h"
#include "Window.h"

#include <map>
#include <string>

class GUISystem
{
public:
	GUISystem(std::shared_ptr<Window> wnd, PersonContainer* persons);
public:
	/* ������� ������ ��� ��������� ���������� */
	
	void Show();					// �������� ���������
	void Hide();					// ������ ���������
	void AddLog(const char* text);	// �������� ���

	/*******************************************/
private: 
	/* ������ ��������� � ��������� ������� */
	
	void SetGUIColors();														// ��������� �������� �����
	void SetPanelWidthAndPosition(int   corner,			float width,			// ��������� ��������� � �������� ������
								  float height,			float x_offset = 0.0f, 
								  float y_offset = 0.0f); 	
	void ShowMenu(); 															// ������� ������ ����
	void ShowLeftSide();														// ����� ������	
	void ShowRightSide();														// ������ ������
	void ShowLeftBottomSide();													// ����� ������ ������
	void ShowBottomPanel();														// ������ ������
	void ShowMouseHelperPanel(std::string info);								// ������ ��� ��������
	void DisableSides();														// ��������� ������� ������

	/****************************************/
private: 
	/* ������ ��������� ���������� ����������� */
	
	void ShowFPSAndGPU();			// �������� ������ � ����������� �� FPS � ����������� �������� 
	void ShowLog();					// �������� ������ ����
	void ShowPersonList();			// �������� ������ ���������� 
	void ShowPersonControl();		// �������� ��������� ��� ���������� ���������

	/*******************************************/
private:
	/* ���������� ��������� ������� */
	
	bool ShowPersonEnum 	  =	true;	// ����� ������ �������� �� �����
	bool ShowPersonSettings   =	true;	// ������ ������ ���������� ����������� ��������
	bool ShowHardwareInfo 	  =	true;	// FPS � ���������� � ����������� ��������
	bool ShowLogs 			  =	true;	// ���
	bool ShowTriggersList 	  =	false;	// ����� ������ ��������� �� �����
	bool ShowTriggersSettings = false;	// ������ ������ ��������� �� �����

	/********************************/
private:
	/* ��������� ���������� */
	
	std::shared_ptr<Window>		   wnd;			// ��������� �� ���� ���������
	std::map<std::wstring, double> gpu_desc;	// �������� ����������� ��������� [first - ���, second - ����� ������]
	AppLog						   applog;		// ���

	/************************/
private:
	/* ��������� �� ���������� */

	PersonContainer* persConPtr;

	/***************************/
private:
	/* ��������������� ���������� ��� ������ � ����������� ���������� */
	
	bool			  DrawingHitBox	    = false;	// ��� ��������� �������� ���������
	bool			  SavingSettings	= false;	// ��� ���������� �������� ���������
	bool			  SettedFirstPoint  = false;	// ���������� ������ ����� hitbox
	bool			  SettedSecondPoint = false;	// ���������� ������ ����� hitbox
	DirectX::XMFLOAT2 firstPoint;					// ���������� ������ ����� hitbox
	DirectX::XMFLOAT2 secondPoint;					// ���������� ������ ����� hitbox
	std::string		  personSelected	  = "";		// ��� ���������� ���������

	/******************************************************************/

	std::string mouseHelpInfo = "";	// ������� ��� ��������
};
